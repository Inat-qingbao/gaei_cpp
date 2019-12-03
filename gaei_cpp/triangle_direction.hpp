#pragma once
#include <vector>
#include <array>
#include "vertex.hpp"
#include "vector_utl.hpp"

namespace gaei{
void triangle_direction_judege(const std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes, std::vector<std::array<int,3>>& as) {
    gaei::vector<double, 3> c, vab, vbc;
    for (int i = 0; i < as.size(); ++i) {
        vab.coord[0] = vertexes[as[i][1]].position.x() - vertexes[as[i][0]].position.x();
        vab.coord[1] = vertexes[as[i][1]].position.y() - vertexes[as[i][0]].position.y();
        vab.coord[2] = vertexes[as[i][1]].position.z() - vertexes[as[i][0]].position.z();
        vbc.coord[0] = vertexes[as[i][2]].position.x() - vertexes[as[i][1]].position.x();
        vbc.coord[1] = vertexes[as[i][2]].position.y() - vertexes[as[i][1]].position.y();
        vbc.coord[2] = vertexes[as[i][2]].position.z() - vertexes[as[i][1]].position.z();
        c =gaei::cross_product(vab, vbc);
        double in = c.coord[2];
        if (in < 0.0) {
            std::swap(as[i][0], as[i][2]);
        }
    }
}

}
