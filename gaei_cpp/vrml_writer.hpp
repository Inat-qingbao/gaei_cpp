#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <memory>
#include <filesystem>

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

    template<class Node, std::enable_if_t<std::is_base_of_v<node_base, std::remove_reference_t<Node>>, int> = 0>
    void push(Node&& node)
    {
        nodes_.push_back(std::make_unique<std::remove_reference_t<Node>>(std::forward<Node>(node)));
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
        out << "#VRML V2.0 utf8\n";
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

template<class Texture = std::nullptr_t>
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
        out << "geometry IndexedFaceSet{\n";
        auto [unused, write] = write_coord(out);
        write_color(out, write);
        //coord_index add later
        out << "}\n";
        return (bool)out;
    }
    auto& data() noexcept { return vertexes_; }
    const auto& data() const noexcept { return vertexes_; }
private:
    bool write_color(std::ostream& out, bool write) const
    {
        if (!write) return true;
        out << "color Color{color[";
        for (auto&& v : vertexes_) {
            out << static_cast<float>(v.color.r() / 255.0) << ' '
                << static_cast<float>(v.color.g() / 255.0) << ' '
                << static_cast<float>(v.color.b() / 255.0);
            out << '\n';
        }
        out << "]}";
        return (bool)out;
    }
    [[nodiscard]]
    std::tuple<bool, bool> write_coord(std::ostream& out) const
    {
        bool is_color_none = false;
        out << "coord Coordinate{";
        out << "point[";
        for (const auto& v : vertexes_) {
            out << v.position.x() << " " << v.position.y() << " " << v.position.z() << '\n';
            is_color_none |= (bool)v.color;
        }
        out << "]\n";
        out << "}\n";
        return { (bool)out, false };
    }
};

struct box {
    vec3f size = { 2,2,2 };
    bool write(std::ostream& out) const
    {
        constexpr auto c = box{};
        out << "geometry Box { \n";
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
        out << "geometry PointSet {\n";
        auto [unused, color] = write_coord(out);
        if (color)
            write_color(out);
        out << "}\n";
        return (bool)out;
    }
private:
    //return:result, write color? 
    [[nodiscard]]
    std::tuple<bool, bool> write_coord(std::ostream& out) const
    {
        bool color = false;
        out << "coord Coordinate {\npoint [\n";
        for (auto&& i : points) {
            out << i.position.x() << ' '
                << i.position.y() << ' '
                << i.position.z() << '\n';
            color |= (bool)i.color;
        }
        out << "]}\n";
        return std::make_tuple((bool)out, color);
    }
    bool write_color(std::ostream& out) const
    {
        out << "color Color {\ncolor [\n";
        for (auto&& i : points) {
            out << i.color.rf() << ' '
                << i.color.gf() << ' '
                << i.color.bf() << '\n';
        }
        out << "]}\n";
        return (bool)out;
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
