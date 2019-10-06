#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <memory>
#include <filesystem>

#include"color.hpp"
#include"vertex.hpp"

namespace gaei::vrml {

namespace detail {

#define GAEI_HAS_MEMBER(member_name)\
template<class T, class = void>\
struct has_member_ ## member_name : std::false_type {};\
\
template<class T>\
struct has_member_ ## member_name<T, std::void_t<decltype(&(T:: ## member_name))>> : std::true_type {};\
template<class T>\
inline constexpr bool has_member_ ## member_name ## _v = has_member_ ## member_name <T>::value;

GAEI_HAS_MEMBER(write);

}

struct node_base {
    virtual ~node_base() = default;
    virtual bool write(std::ostream& out) const = 0;
};

class vrml_writer {
    std::list<std::unique_ptr<node_base>> nodes_;
public:

    template<class Node>
    void push(Node&& node)
    {
        nodes_.push_back(std::make_unique<std::remove_reference_t<Node>>(std::forward<Node>(node)));
    }
    template<class Node, class ...Args>
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
    virtual bool write(std::ostream& out) const override {
		bool ret = true;
		out << "Shape{";
		ret &= write_geometry(out);
		ret &= write_appearance(out);
		out << "}\n";
		return ret;
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
        return (bool)out
    }
    virtual bool write_appearance(std::ostream& out) const override
    {
        appearance_.write(out);
        return (bool)out;
    }
};

struct material {
    float ambient_intensity;
    color diffuse_color;
    color specular_color;
    float shininess;
    color emissive_color;
    float transparency = 0;
    bool write(std::ostream& out) const
    {
        out << "Material {";
        out << "ambientIntensity " << ambient_intensity << '\n';
        out << "diffuseColor " << static_cast<float>(diffuse_color.r() / 255.0) << ' '
            << static_cast<float>(diffuse_color.g() / 255.0) << ' '
            << static_cast<float>(diffuse_color.b() / 255.0) << '\n';

        out << "specularColor " << static_cast<float>(specular_color.r() / 255.0) << ' '
            << static_cast<float>(specular_color.g() / 255.0) << ' '
            << static_cast<float>(specular_color.b() / 255.0) << '\n';

        out << "emissiveColor " << static_cast<float>(emissive_color.r() / 255.0) << ' '
            << static_cast<float>(emissive_color.g() / 255.0) << ' '
            << static_cast<float>(emissive_color.b() / 255.0) << '\n';

        out << "shininess " << shininess << '\n';
        out << "transparency " << shininess << '\n';
        out << "}\n";
        return (bool)out;
    }
};

struct texture_transform {
    vec2f center;
    float rotation;
    vec2f scale;
    vec2f translation;
    bool write(std::ostream& out) const
    {
        return false;
    }
};

template<class Texture = std::nullptr_t>
struct appearance {
    material mate;
    Texture texture;
    texture_transform transform;
    template<
        class T = Texture,
        std::enable_if_t<detail::has_member_write_v<T>>* = nullptr>
    bool write(std::ostream& out) const
    {
        out << "appearance Appearance {\n";
        out.write(out);
        texture.write(out);
        transform.write(out);
        out << "}\n";
        return (bool)out
    }
    template<
        class T = Texture,
        std::enable_if_t<! detail::has_member_write_v<T>>* = nullptr>
    bool write(std::ostream& out) const
    {
        out << "appearance Appearance {\n";
        out.write(out);
        out << "}\n";
        return (bool)out
    }
};

// こいつはvertexのvectorを保持するべき。
// なのでvertexのvectorを受け取る関数が必要。
class indexed_face_set {
	std::vector<gaei::vertex<gaei::vec3f, gaei::color>> vertexes_;
public:
	bool write(std::ostream& out) const {
		out << "geometry IndexedFaceSet{";
		write_coord(out);
		write_color(out);
		//coord_index add later
		out << "}\n";
		return (bool)out;
	}
    auto& data() noexcept { return vertexes_; }
    const auto& data() const noexcept { return vertexes_; }
private:
    bool write_color(std::ostream& out) const {
		out << "color Color{color[";
        for(auto&& v : vertexes_){
			out << static_cast<float>(v.color.r() / 255.0) << ' ';
			out << static_cast<float>(v.color.g() / 255.0) << ' ';
			out << static_cast<float>(v.color.b() / 255.0);
			out << '\n';
        }
		out << "]}";
		return (bool)out;
    }
	bool write_coord(std::ostream& out) const {
		out << "coord Coordinate{";
		out << "point[";
		for (const auto& v : vertexes_) {
			out << v.position.x() << " " << v.position.y() << " " << v.position.z() << '\n';
		}
		out << "]\n";
		out << "}\n";
		return (bool)out;
	}
};

class box {
    vec3f size;
    bool write(std::ostream& out) const
    {
        out << "Box { \n";
        out << "size " << size.x() << ' '
            << size.y() << ' '
            << size.z() << '\n';
        out << "}\n";
        return (bool)out;
    }
};

struct transform : public node_base {
    vec3f translation = { 0, 0, 0 };
    vector<float, 4> rotation = {1, 0, 0, 0};
    vec3f scale = {1, 1, 1};
    vector<float, 4> scale_orientation = { 0,0,1,0 };
    vec3f bbox_center = { 0,0,0 };
    vec3f bbox_size = {-1, -1, -1};
    std::list<std::unique_ptr<node_base>> children;
    bool write(std::ostream& out) const override
    {
        out << "Transform {\n";
        out << "translation "
            << translation.x() << ' '
            << translation.y() << ' '
            << translation.z() << '\n';
        out << "rotation "
            << translation.x() << ' '
            << translation.y() << ' '
            << translation.z() << ' '
            << translation.coord[3] << '\n';
        out << "scale "
            << scale.x() << ' '
            << scale.y() << ' '
            << scale.z() << '\n';
        out << "scaleOrientation "
            << translation.x() << ' '
            << translation.y() << ' '
            << translation.z() << ' '
            << translation.coord[3] << '\n';
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
