#pragma once
#include <vector>
#include <iostream>
#include <array>
#include "vertex.hpp"
#include "vector_utl.hpp"

namespace gaei{
    
void triangle_direction_judege(std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes, std::vector<std::array>& as) {
    std::vector<double, 3> c, vab, vbc;
    for (int i = 0; i < as.size; i++) {
        vab.coord[0] = vertexes[as[i][1]].x() - vertexes[as[i][0]].x();
        vab.coord[1] = vertexes[as[i][1]].y() - vertexes[as[i][0]].y();
        vab.coord[2] = vertexes[as[i][1]].z() - vertexes[as[i][0]].z();
        vbc.coord[0] = vertexes[as[i][2]].x() - vertexes[as[i][1]].x();
        vbc.coord[1] = vertexes[as[i][2]].y() - vertexes[as[i][1]].y();
        vbc.coord[2] = vertexes[as[i][2]].z() - vertexes[as[i][1]].z();
        c =gaei::cross_product(vab, vbc);
        auto in = c.coord[2];
        if (in > 0.0) {
            std::swap(as[i][0], as[i][2]);
        }
    }
}

}
