#include<iostream>
#include<cmath>
#include<vector>
#include"vertex.hpp"


namespace gaei {
	/*まず最大値と最小値を求める*/
	const std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes;
	/*for (const auto& vertex : vertexes) {
		vertex.position.x();
		vertex.position.y();
		vertex.position.z();
	}*/
	double minX = *std::min_element(vertexes.begin(), vertexes.end()) 
		[](const auto& v1, const auto& v2) { return v1.position.x() < v2.position.x(); });
	double minY = *std::min_element(vertexes.begin(), vertexes.end())
		[](const auto& v1, const auto& v2) { return v1.position.y() < v2.position.y(); });
	double minZ = *std::min_element(vertexes.begin(), vertexes.end())
		[](const auto& v1, const auto& v2) { return v1.position.z() < v2.position.z(); });
	double maxX = *std::max_element(vertexes.begin(), vertexes.end())
		[](const auto& v1, const auto& v2) { return v1.position.x() < v2.position.x(); });
	double maxY = *std::max_element(vertexes.begin(), vertexes.end())
		[](const auto& v1, const auto& v2) { return v1.position.y() < v2.position.y(); });
	double maxZ = *std::max_element(vertexes.begin(), vertexes.end())
		[](const auto& v1, const auto& v2) { return v1.position.z() < v2.position.z(); });

	
}
