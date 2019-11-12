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
}// namespace detail

struct node_base {
    virtual ~node_base() = default;
    virtual bool write(std::ostream& out) const = 0;
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

    bool write(std::ostream& out) const noexcept
    {
        write_headder(out);
        for (auto& i : nodes_) i->write(out);
        return (bool)out;
    }
    bool write(const std::filesystem::path& path) const noexcept
    {
        std::ofstream out(path);
        return write(out);
    }
private:
    bool write_headder(std::ostream& out) const
    {
        constexpr char header[] = "#VRML V2.0 utf8\n";
        out.write(header, sizeof(header) - 1);
        return (bool)out;
    }
};

class shape_base : public node_base {
public:
    // for safe destruction, the destructor should be virtual;
    virtual ~shape_base() = default;
    shape_base() = default;

    // 外(主にvrml_writer)から呼び出されてshapeノードを出力する関数
    virtual bool write(std::ostream& out) const override
    {
        out << "Shape{";
        write_geometry(out);
        write_appearance(out);
        out << "}\n";
        return (bool)out;
    }
protected:
    // 継承先の具体的な`shape`から呼び出されてGeometryノード(IndexedFaceSetやBoxなど)を書き込む純粋仮想関数
    virtual bool write_geometry(std::ostream& out) const = 0;
    // 継承先の具体的な`shape`から呼び出されてAppearanceノードを書き込む純粋仮想関数
    virtual bool write_appearance(std::ostream& out) const = 0;
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
    virtual bool write_geometry(std::ostream& out) const override
    {
        geometry_.write(out);
        return (bool)out;
    }
    virtual bool write_appearance(std::ostream& out) const override
    {
        appearance_.write(out);
        return (bool)out;
    }
};

struct material {
    float ambient_intensity = 0.2f;
    color diffuse_color = colors::none;
    color specular_color = colors::none;
    float shininess = 0.2f;
    color emissive_color = colors::none;
    float transparency = 0;
    bool write(std::ostream& out) const
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
        return (bool)out;
    }
};

struct texture_transform {
    vec2f center = { 0, 0 };
    float rotation = 0;
    vec2f scale = { 1, 1 };
    vec2f translation = { 0, 0 };
    bool write([[maybe_unused]] std::ostream& out) const
    {
        return false;
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
        bool write(std::ostream& out) const
    {
        out << "appearance Appearance {\n";
        mate.write(out);
        texture.write(out);
        transform.write(out);
        out << "}\n";
        return (bool)out;
    }
    template<
        class T = Texture,
        std::enable_if_t<! gaei::detail::has_member_write_v<T>>* = nullptr>
    bool write(std::ostream& out) const
    {
        out << "appearance Appearance {\n";
        mate.write(out);
        out << "}\n";
        return (bool)out;
    }
};

class indexed_face_set {
    std::vector<gaei::vertex<gaei::vec3f, gaei::color>> vertexes_;
public:
    bool write(std::ostream& out) const
    {
        std::string buffer;
        bool success = true;
        buffer.reserve(vertexes_.size() * 128);
        buffer.append("geometry IndexedFaceSet{\n");
        auto [s, write] = write_coord(buffer);
        success &= s;
        success &= write_color(buffer, write);
        //coord_index add later
        buffer.append("}\n");
        out.write(buffer.data(), buffer.size());
        return (bool)out && success;
    }
    auto& data() noexcept { return vertexes_; }
    const auto& data() const noexcept { return vertexes_; }
private:
    bool write_color(std::string& out, bool write) const
    {
        if (!write) return true;
        out.append("color Color{color[");
        for (auto&& v : vertexes_) {
            if (!to_vrml(v.color, out)) return false;
            out.push_back('\n');
        }
        out.append("]}");
        return true;
    }
    [[nodiscard]]
    std::tuple<bool, bool> write_coord(std::string& out) const
    {
        bool is_color_none = false;
        out.append("coord Coordinate{");
        out.append("point[");
        for (const auto& v : vertexes_) {
            if (!to_vrml(v.position, out)) return { false, false };
            out.push_back('\n');
            is_color_none |= (bool)v.color;
        }
        out.append("]\n");
        out.append("}\n");
        return { true, is_color_none };
    }
};

struct box {
    vec3f size = { 2,2,2 };
    bool write(std::ostream& out) const
    {
        constexpr auto c = box{};
        out << "geometry Box {\n";
        if(size != c.size)
            out << "size " << size.x() << ' '
                << size.y() << ' '
                << size.z() << '\n';
        out << "}\n";
        return (bool)out;
    }
};

struct point_set {
    std::vector<gaei::vertex<gaei::vec3f, gaei::color>> points;
    bool write(std::ostream& out) const
    {
        std::string buffer;
        buffer.reserve(points.size() * 64);
        out << "geometry PointSet {\n";
        auto [success, color] = write_coord(buffer);
        if (color)
            success &= write_color(buffer);
        out.write(buffer.data(), buffer.size());
        out << "}\n";
        return (bool)out & success;
    }
private:
    //return:result, write color? 
    [[nodiscard]]
    std::tuple<bool, bool> write_coord(std::string& out) const
    {
        bool color = false;
        bool is_success = true;
        out.append("coord Coordinate {\npoint [\n");
        for (auto&& i : points) {
            is_success &= to_vrml(i.position, out).is_ok();
            out.append("\n");
            color |= (bool)i.color;
        }
        out.append("]}\n");
        return std::make_tuple(is_success, color);
    }
    bool write_color(std::string& out) const
    {
        bool is_success = true;
        out.append("color Color {\ncolor [\n");
        for (auto&& i : points) {
            is_success &= to_vrml(i.color, out).is_ok();
            out.append("\n");
        }
        out.append("]}\n");
        return is_success;
    }
};

struct transform : public node_base {
    vec3f translation = { 0, 0, 0 };
    vector<float, 4> rotation = { 1, 0, 0, 0 };
    vec3f scale = { 1, 1, 1 };
    vector<float, 4> scale_orientation = { 0,0,1,0 };
    vec3f bbox_center = { 0,0,0 };
    vec3f bbox_size = { -1, -1, -1 };
    std::list<std::unique_ptr<node_base>> children;
    bool write(std::ostream& out) const override
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
            i->write(out);
        }
        out << "]\n}\n";
        return (bool)out;
    }
};

}
