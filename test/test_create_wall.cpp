﻿#include"wall.hpp"
#include"ouchitest.hpp"
OUCHI_TEST_CASE(test_wall) {
    std::vector<gaei::vertex<gaei::vec3f, gaei::color>> vertexes;
    std::vector<long> s;
    vertexes.resize(3);
    vertexes[0].position = gaei::vec3f{ {0,0,0} };
    vertexes[1].position = gaei::vec3f{ {2,2,-0.1} };
    vertexes[2].position = gaei::vec3f{ {1,1,4} };
    s = { 0,1,2,-1 };
    auto ans = s;
    ans.insert(ans.end(), { 7,9,4,6,-1, 8,7,6,5,-1, 10,8,5,3,-1, 9,10,3,4,-1, 8,10,9,7,-1 });

    gaei::bounding_box(vertexes);
    gaei::create_wall(vertexes, s);
    OUCHI_CHECK_EQUAL(s, ans);
}
