#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <memory>
#include <filesystem>
#include <variant>  // for std::monostate

#include "vrml_helper.hpp"
#include "color.hpp"
#include "vertex.hpp"
#include "meta.hpp"

#include "ouchilib/result/result.hpp"

namespace gaei::vrml {

namespace detail {

template<class T, class ...Printable>
bool write_if_different(const T& default_value,
                        const T& actual_value,
                        std::ostream& out,
                        Printable&& ...pt)
{
    if (default_value == actual_value)
        return false;
    (out << ... << pt);
    return true;
}

std::string stream_error_message(const std::ios_base& s)
{
    using namespace std::string_literals;
    return s.good() ? ""
        : s.bad() ? "irrecoverable stream error!"
        : s.fail() ? "input/output operation failed! (formatting or extraction error)"
        : "associated input sequence has reached end-of-file";
}

ouchi::result::result<std::monostate, std::string> streamtoresult(const std::ios_base& s)
{
    using namespace std::string_literals;
    using namespace ouchi::result;
    using res = ouchi::result::result<std::monostate, std::string>;
    return s.good() ? res{ ok{ std::monostate{} } }
        : s.bad() ? res{ err{ "irrecoverable stream error!" } }
        : s.fail() ? res{ err{"input/output operation failed! (formatting or extraction error)"} }
        : res{ err{ "associated input sequence has reached end-of-file" } };
}

}// namespace detail

struct node_base {
    virtual ~node_base() = default;
    virtual ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const = 0;
};

class vrml_writer {
    std::list<std::unique_ptr<node_base>> nodes_;
public:

    template<class Node, std::enable_if_t<std::is_base_of_v<node_base, remove_cvref_t<Node>>, int> = 0>
    void push(Node&& node)
    {
        nodes_.push_back(std::make_unique<remove_cvref_t<Node>>(std::forward<Node>(node)));
    }
    template<class Node, class ...Args, std::enable_if_t<std::is_base_of_v<node_base, Node>, int> = 0>
    void push(Args&& ...args)
    {
        nodes_.push_back(std::make_unique<Node>(std::forward<Args>(args)...));
    }
    auto& data() noexcept { return nodes_; }
    const auto& data() const noexcept { return nodes_; }

    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const noexcept
    {
        using namespace ouchi::result;
        auto r = write_headder(out);
        for (auto& i : nodes_) r = r && i->write(out);
        return r;
    }
    ouchi::result::result<std::monostate, std::string>
    write(const std::filesystem::path& path) const noexcept
    {
        std::ofstream out(path);
        return write(out);
    }
private:
    ouchi::result::result<std::monostate, std::string>
    write_headder(std::ostream& out) const
    {
        using namespace ouchi::result;
        constexpr char header[] = "#VRML V2.0 utf8\n";
        out.write(header, sizeof(header) - 1);
        if (out.good()) return ok{ std::monostate{} };
        return err{ detail::stream_error_message(out) };
    }
};

class shape_base : public node_base {
public:
    // for safe destruction, the destructor should be virtual;
    virtual ~shape_base() = default;
    shape_base() = default;

    // 外(主にvrml_writer)から呼び出されてshapeノードを出力する関数
    virtual 
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const override
    {
        out << "Shape{";
        auto r = write_geometry(out);
        r = r && write_appearance(out);
        r = r && detail::streamtoresult(out << "}\n");
        return r;
    }
protected:
    // 継承先の具体的な`shape`から呼び出されてGeometryノード(IndexedFaceSetやBoxなど)を書き込む純粋仮想関数
    virtual ouchi::result::result<std::monostate, std::string>
    write_geometry(std::ostream& out) const = 0;
    // 継承先の具体的な`shape`から呼び出されてAppearanceノードを書き込む純粋仮想関数
    virtual ouchi::result::result<std::monostate, std::string>
    write_appearance(std::ostream& out) const = 0;
};

template<class Geometry, class Appearance>
class shape : public shape_base {
    Geometry geometry_;
    Appearance appearance_;
public:
    virtual ~shape() = default;
    shape() = default;

    Geometry& geometry() noexcept { return geometry_; }
    const Geometry& geometry() const noexcept { return geometry_; }
    Appearance& appearance() noexcept { return appearance_; }
    const Appearance& appearance() const noexcept { return appearance_; }
protected:
    ouchi::result::result<std::monostate, std::string>
    write_geometry(std::ostream& out) const override
    {
        return geometry_.write(out);
    }
    ouchi::result::result<std::monostate, std::string>
    write_appearance(std::ostream& out) const override
    {
        return appearance_.write(out);
    }
};

struct material {
    float ambient_intensity = 0.2f;
    color diffuse_color = colors::none;
    color specular_color = colors::none;
    float shininess = 0.2f;
    color emissive_color = colors::none;
    float transparency = 0;

    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const
    {
        constexpr auto default_value = material{};
        out << "material Material {";
        detail::write_if_different(default_value.ambient_intensity,
                      ambient_intensity,
                      out, "ambientIntensity ", ambient_intensity, '\n');
        detail::write_if_different(default_value.diffuse_color,
                      diffuse_color,
                      out, "diffuseColor ",
                      diffuse_color.rf(), diffuse_color.gf(), diffuse_color.bf(), '\n');
        detail::write_if_different(default_value.specular_color,
                      specular_color,
                      out, "specularColor ",
                      specular_color.rf(), specular_color.gf(), specular_color.bf(), '\n');
        detail::write_if_different(default_value.emissive_color,
                      emissive_color,
                      out, "emissiveColor ",
                      emissive_color.rf(), emissive_color.gf(), emissive_color.bf(), '\n');
        detail::write_if_different(default_value.shininess, shininess,
                      out, "shininess ", shininess, '\n');
        detail::write_if_different(default_value.transparency, transparency,
                      out, "transparency ", transparency, '\n');
        out << "}\n";
        return out.good()
            ? ouchi::result::result<std::monostate, std::string>{ouchi::result::ok{ std::monostate{} }}
            : ouchi::result::result<std::monostate, std::string>{ ouchi::result::err{detail::stream_error_message(out)} };
    }
};

struct texture_transform {
    vec2f center = { 0, 0 };
    float rotation = 0;
    vec2f scale = { 1, 1 };
    vec2f translation = { 0, 0 };
    ouchi::result::result<std::monostate, std::string>
    write([[maybe_unused]] std::ostream& out) const
    {
        return ouchi::result::ok{ std::monostate{} };
    }
};

template<class Texture = std::monostate>
struct appearance {
    material mate;
    Texture texture = Texture{};
    texture_transform transform = texture_transform{};
    template<
        class T = Texture,
        std::enable_if_t<gaei::detail::has_member_write_v<T>>* = nullptr>
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const
    {
        using namespace ouchi::result;
        result<std::monostate, std::string> res = ok{ std::monostate{} };
        res = res && detail::streamtoresult(out << "appearance Appearance {\n");
        res = res && mate.write(out);
        res = res && texture.write(out);
        res = res && transform.write(out);
        res = res && detail::streamtoresult(out << "}\n");
        return res;
    }
    template<
        class T = Texture,
        std::enable_if_t<! gaei::detail::has_member_write_v<T>>* = nullptr>
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const
    {
        using namespace ouchi::result;
        result<std::monostate, std::string> res = ok{ std::monostate{} };
        res = res && detail::streamtoresult((out << "appearance Appearance {\n"));
        res = res && mate.write(out);
        res = res && detail::streamtoresult(out << "}\n");
        return res;
    }
};

struct indexed_face_set {
    std::vector<gaei::vertex<>> coord_;
    std::vector<long> coord_index_;
    bool ccw = true;
    bool convex = false;
    bool solid = false;
public:
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const
    {
        std::string buffer;
        ouchi::result::result<std::monostate, std::string> success = ouchi::result::ok{ std::monostate{} };
        buffer.reserve(coord_.size() * 128);
        buffer.append("geometry IndexedFaceSet{\n");
        buffer.append("\nccw "); buffer.append(ccw ? "TRUE" : "FALSE");
        buffer.append("\nconvex ");buffer.append(convex ? "TRUE" : "FALSE");
        buffer.append("\nsolid ");buffer.append(solid ? "TRUE" : "FALSE");
        buffer.append("\n");
        auto [s, write] = write_coord(buffer);
        success = success && s;
        success = success && write_color(buffer, write);
        //coord_index add later
        buffer.append("}\n");
        out.write(buffer.data(), buffer.size());
        return success
            ? ouchi::result::result<std::monostate, std::string>{ouchi::result::ok{ std::monostate{} }}
            : ouchi::result::result<std::monostate, std::string>{ ouchi::result::err{detail::stream_error_message(out)} };
    }
    auto& data() noexcept { return coord_; }
    const auto& data() const noexcept { return coord_; }
private:
    ouchi::result::result<std::monostate, std::string>
    write_color(std::string& out, bool write) const
    {
        if (!write) return ouchi::result::ok{ std::monostate{} };
        out.append("color Color{color[");
        for (auto&& v : coord_) {
            if (auto r = to_vrml(v.color, out); !r) return ouchi::result::err{std::make_error_code(r.unwrap_err()).message()};
            out.push_back('\n');
        }
        out.append("]}");
        return ouchi::result::ok{ std::monostate{} };
    }
    [[nodiscard]]
    std::tuple<ouchi::result::result<std::monostate, std::string>, bool> write_coord(std::string& out) const
    {
        bool is_color_none = false;
        out.append("coord Coordinate{");
        out.append("point[");
        for (const auto& v : coord_) {
            if (auto r = to_vrml(v.position, out); !r) return { ouchi::result::err{std::make_error_code(r.unwrap_err()).message()}, false };
            out.push_back('\n');
            is_color_none |= (bool)v.color;
        }
        out.append("]\n");
        out.append("}\n");
        out.append("coordIndex [\n");
        for(auto&& idx : coord_index_){
            char buffer[16] = {};
            std::to_chars(buffer, buffer+16, idx);
            out.append(buffer);
            out.push_back(' ');
        }
        out.append("]");
        return { ouchi::result::ok{ std::monostate{} }, is_color_none };
    }
};

struct box {
    vec3f size = { 2,2,2 };
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const
    {
        constexpr auto c = box{};
        out << "geometry Box {\n";
        if(size != c.size)
            out << "size " << size.x() << ' '
                << size.y() << ' '
                << size.z() << '\n';
        out << "}\n";
        return out.good()
            ? ouchi::result::result<std::monostate, std::string>{ouchi::result::ok{ std::monostate{} }}
            : ouchi::result::result<std::monostate, std::string>{ ouchi::result::err{detail::stream_error_message(out)} };
    }
};

struct point_set {
    std::vector<gaei::vertex<>> points;
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const
    {
        std::string buffer;
        buffer.reserve(points.size() * 64);
        out << "geometry PointSet {\n";
        auto [success, color] = write_coord(buffer);
        if (color)
            success = success && write_color(buffer);
        success = success && detail::streamtoresult(out.write(buffer.data(), buffer.size()));
        success = success && detail::streamtoresult((out << "}\n"));
        return out.good()
            ? ouchi::result::result<std::monostate, std::string>{ouchi::result::ok{ std::monostate{} }}
            : ouchi::result::result<std::monostate, std::string>{ ouchi::result::err{detail::stream_error_message(out)} };
    }
private:
    //return:result, write color? 
    [[nodiscard]]
    std::tuple<ouchi::result::result<std::monostate, std::string>, bool> write_coord(std::string& out) const
    {
        bool color = false;
        ouchi::result::result<std::monostate, std::string> is_success = ouchi::result::ok{ std::monostate{} };
        out.append("coord Coordinate {\npoint [\n");
        for (auto&& i : points) {
            is_success = is_success && to_vrml(i.position, out).map([](auto)->std::monostate {return {}; }).map_err([](std::errc c) {return std::make_error_code(c).message(); });
            out.append("\n");
            color |= (bool)i.color;
        }
        out.append("]}\n");
        return std::make_tuple(is_success, color);
    }
    ouchi::result::result<std::monostate, std::string>
    write_color(std::string& out) const
    {
        ouchi::result::result<std::monostate, std::string> is_success = ouchi::result::ok{ std::monostate{} };
        out.append("color Color {\ncolor [\n");
        for (auto&& i : points) {
            is_success = is_success && to_vrml(i.color, out).map([](auto)->std::monostate {return {}; }).map_err([](std::errc c) {return std::make_error_code(c).message(); });
            out.append("\n");
        }
        out.append("]}\n");
        return is_success;
    }
};

struct [[deprecated("not safe because error handling is not satisfied. there is no substitute")]]
    transform : public node_base
{
    vec3f translation = { 0, 0, 0 };
    vector<float, 4> rotation = { 1, 0, 0, 0 };
    vec3f scale = { 1, 1, 1 };
    vector<float, 4> scale_orientation = { 0,0,1,0 };
    vec3f bbox_center = { 0,0,0 };
    vec3f bbox_size = { -1, -1, -1 };
    std::list<std::unique_ptr<node_base>> children;
    ouchi::result::result<std::monostate, std::string>
    write(std::ostream& out) const override
    {
        out << "Transform {\n";
        out << "translation "
            << translation.x() << ' '
            << translation.y() << ' '
            << translation.z() << '\n';
        out << "rotation "
            << rotation.x() << ' '
            << rotation.y() << ' '
            << rotation.z() << ' '
            << rotation.coord[3] << '\n';
        out << "scale "
            << scale.x() << ' '
            << scale.y() << ' '
            << scale.z() << '\n';
        out << "scaleOrientation "
            << scale_orientation.x() << ' '
            << scale_orientation.y() << ' '
            << scale_orientation.z() << ' '
            << scale_orientation.coord[3] << '\n';
        out << "bboxCenter "
            << bbox_center.x() << ' '
            << bbox_center.y() << ' '
            << bbox_center.z() << '\n';
        out << "bboxSize "
            << bbox_size.x() << ' '
            << bbox_size.y() << ' '
            << bbox_size.z() << '\n';
        out << "children [\n";
        for (const auto& i : children) {
            (void)(i->write(out));
        }
        out << "]\n}\n";
        return out.good()
            ? ouchi::result::result<std::monostate, std::string>{ouchi::result::ok{ std::monostate{} }}
            : ouchi::result::result<std::monostate, std::string>{ ouchi::result::err{detail::stream_error_message(out)} };
    }
};

}
