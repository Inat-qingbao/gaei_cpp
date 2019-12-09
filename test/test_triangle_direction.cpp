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
    vertexes[1].position = gaei::vec3f{ {2,1.9,-0.1} };
    vertexes[2].position = gaei::vec3f{ {1,1,4} };
    gaei::triangle_direction_judege(vertexes, as);
    OUCHI_CHECK_EQUAL(as,ans);
    as[0][0] = 2;
    as[0][2] = 0;
    gaei::triangle_direction_judege(vertexes, as);
    OUCHI_CHECK_EQUAL(as, ans);
}
