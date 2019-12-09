#pragma once
#include <vector>
#include <array>
#include "vertex.hpp"
#include "vector_utl.hpp"

namespace gaei{
inline void triangle_direction_judege(const std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes, std::vector<std::array<int,3>>& as) {
    gaei::vec3f c, vab, vbc;
    for (int i = 0; i < as.size(); ++i) {
        vab = vertexes[as[i][1]].position - vertexes[as[i][0]].position;
        vbc = vertexes[as[i][2]].position - vertexes[as[i][1]].position;
        c =gaei::cross_product(vab, vbc);
        double in = c.coord[2];
        if (in < 0.0) {
            std::swap(as[i][0], as[i][2]);
        }
    }
}

}
