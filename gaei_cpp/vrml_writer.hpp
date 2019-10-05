#pragma once
#include<iostream>
#include<vector>
#include<fstream>

#include"color.hpp"
#include"vertex.hpp"

namespace gaei::vrml {
class header {
public:
    // ������o�͊֐��̋��ʃC���^�[�t�F�[�X�Ƃ��悤
	bool write(std::ostream& out) {
		out << "#VRML V2.0 utf8\n";
		return true;
	}
};

class vrml_writer {

};

class shape_base {
public:
    // for safe destruction, the destructor should be virtual;
	virtual ~shape_base() = default;
	shape_base() = default;

    // �O(���vrml_writer)����Ăяo�����shape�m�[�h���o�͂���֐�
    virtual bool write(std::ostream& out) const {
		bool ret = true;
		out << "Shape{";
		ret &= write_geometry(out);
		ret &= write_appearance(out);
		out << "}\n";
		return ret;
	}
protected:
    // �p����̋�̓I��`shape`����Ăяo�����Geometry�m�[�h(IndexedFaceSet��Box�Ȃ�)���������ޏ������z�֐�
	virtual bool write_geometry(std::ostream& out) const = 0;
    // �p����̋�̓I��`shape`����Ăяo�����Appearance�m�[�h���������ޏ������z�֐�
	virtual bool write_appearance(std::ostream& out) const = 0;
};

template<class Geometry, class Appearance>
class shape : public shape_base {
	Geometry geometry_;
	Appearance appearance_;
public:
	virtual ~shape() = default;
	shape() = default;
    
protected:
    virtual bool write_geometry(std::ostream& out) const override {
		bool ret = true;
        ret &&= geometry_.write(out);
        ret &&= appearance_.write(out);
    }
};

// ������vertex��vector��ێ�����ׂ��B
// �Ȃ̂�vertex��vector���󂯎��֐����K�v�B
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

}
