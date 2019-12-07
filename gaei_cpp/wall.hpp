#pragma once
#include<iostream>
#include<vector>
#include"vertex.hpp"

namespace gaei {
inline void bounding_box(std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes) {
    auto max_x, max_y, min_x, min_y, min_z;
    max_x = vertexes[0].position.x();
    min_x = vertexes[0].position.x();
    max_y = vertexes[0].position.y();
    min_y = vertexes[0].position.y();
    min_z = vertexes[0].position.z();
    for (int i = 0; i < vertexes.size(); i++) {
        if (max_x < vertexes[i].position.x()) {
            max_x = vertexes[i].position.x();
        }
        if (min_x > vertexes[i].position.x()) {
            min_x = vertexes[i].position.x();
        }
        if (max_y < vertexes[i].position.y()) {
            max_y = vertexes[i].position.y();
        }
        if (min_y > vertexes[i].position.y()) {
            min_y = vertexes[i].position.y();
        }
        if (min_z < vertexes[i].position.z()) {
            min_z = vertexes[i].position.z();
        }
    }
    vertexes.push_back({ {max_x,max_y,min_z},{} });
    vertexes.push_back({ {max_x,min_y,min_z},{} });
    vertexes.push_back({ {min_x,max_y,min_z},{} });
    vertexes.push_back({ {min_x,min_y,min_z},{} });
}

}
