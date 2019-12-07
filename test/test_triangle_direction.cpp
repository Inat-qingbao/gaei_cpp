#include"triangle_direction.hpp"
#include"ouchitest.hpp"
OUCHI_TEST_CASE(test_triangle_direction){
    std::vector<std::array<int,3>> as;
    std::vector<gaei::vertex<gaei::vec3f, gaei::color>> vertexes;
    as.resize(3);
    as[0][0] = 0;
    as[0][1] = 1;
    as[0][2] = 2;
    std::vector<std::array<int,3>> ans;
    ans = as;
    vertexes.resize(3);
    vertexes[0].position = gaei::vec3f{ {0,0,0} };
    vertexes[1].position = gaei::vec3f{ {3,0,3} };
    vertexes[2].position = gaei::vec3f{ {3,1,0} };
    gaei::triangle_direction_judege(vertexes, as);
    OUCHI_CHECK_EQUAL(as[0][0],ans[0][0]);
    as[0][0] = 2;
    as[0][2] = 0;
    gaei::triangle_direction_judege(vertexes, as);
    OUCHI_CHECK_EQUAL(as[0][0], ans[0][0]);
}
