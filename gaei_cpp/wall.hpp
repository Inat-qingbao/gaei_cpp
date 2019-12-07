#pragma once
#include<iostream>
#include<vector>
#include"vertex.hpp"

namespace gaei {
inline void bounding_box(std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes) {
    typename vec3f::value_type max_x, max_y, min_x, min_y, min_z;
    max_x = vertexes[0].position.x();
    min_x = vertexes[0].position.x();
    max_y = vertexes[0].position.y();
    min_y = vertexes[0].position.y();
    min_z = vertexes[0].position.z();
    for (unsigned i = 1; i < vertexes.size(); i++) {
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

inline void create_wall(std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vertexes,std::vector<long>& S) {
    long end = vertexes.size()-1;
    typename vec3f::value_type min_z = (vertexes[end-2].position.x()-vertexes[end].position.x())/2.0;
    for (int i = end-4; i <end ; i++) {
        vertexes.push_back({ {vertexes[i].position.x(),vertexes[i].position.y(),min_z},{} });
    }
    end = vertexes.size();
    S.insert(S.end(),{ end,end - 7,end - 6,end - 1,-1,end - 1,end - 6,end - 4,end - 3,-1,end - 3,end - 4,end - 5,end - 2,-1,end - 2,end - 5,end - 7,end,-1,end - 3,end - 2,end,end - 1,-1 });
}

}
