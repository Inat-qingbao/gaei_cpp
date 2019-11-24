#include<iostream>
#include<cmath>
#include<vector>
#include"vertex.hpp"
#include"delaunator.hpp"


namespace gaei {
	void delaunay(std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes) {
		std::vector<double> coords;
		for (const auto& vertex:vertexes) {
			int i = 0;
			coords[i] = vertex.position.x();
			coords[i +1] = vertex.position.y();
			i += 2;
		}
		delaunator::Delaunator d(coords);
		
}
