#include<iostream>
#include<vector>
#include<fstream>

#include"color.hpp"
#include"vertex.hpp"

namespace gaei {
class vrml_writer {
	public:
		void header(std::ostream& out) {
			out<<u8"#VRML V2.0 utf8\n;
		}
		void shape(std::ostream& out) {
			out << u8"Shape{";
			index_face_set();
			out << u8"}\n";
		}
		void index_face_set(std::ostream& out) {
			out << u8"geometry IndexedFaceSet{";
			coord_coordinate();
			//coord_index add later
			out <<u8"}\n"
		}
		void coord(std::ostream& out) {
			out << u8"coord Coordinate";
			point();
			out<<u8"}\n"
		}
		void point(std::ostream& out, const std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes) {
			out << u8"point[";
			for(const auto& v : vertexes){
				out << v.position.x() << u8" " << v.position.y() << u8" " << v.position.z() << '\n';
			}
			out<<u8"]\n"
		}
};

}