#include"wall.hpp"
#include"ouchitest.hpp"
OUCHI_TEST_CASE(test_wall) {
    std::vector<gaei::vertex<gaei::vec3f, gaei::color>> vertexes;
    std::vector<long> s;
    vertexes.resize(3);
    vertexes[0].position = gaei::vec3f{ {0,4,0} };
    vertexes[1].position = gaei::vec3f{ {-3,0,-3} };
    vertexes[2].position = gaei::vec3f{ {-3,0,3} };
    s = { 0,1,2,-1 };
    auto ans = s;
    ans.insert(ans.end(), { 11,4,5,10,-1, 10,5,7,8,-1, 8,7,6,9,-1, 9,6,4,11,-1, 8,9,11,10,-1 });

    gaei::bounding_box(vertexes);
    gaei::create_wall(vertexes, s);
    OUCHI_CHECK_EQUAL(s,ans);
}
