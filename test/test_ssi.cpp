#include "ouchitest.hpp"
#include "vertex.hpp"
#include "surface_structure_isolate.hpp"

namespace {

//   0 1 2 3
// 0 c * * O
// 1 * o o O
// 2 c * * O
constexpr gaei::vertex<> vs[] = {
    {gaei::vec3f{0, 0, 0}, gaei::color{}},
    {gaei::vec3f{0, 1, 0}, gaei::color{}},
    {gaei::vec3f{0, 2, 0}, gaei::color{}},
    {gaei::vec3f{0, 3, 20}, gaei::color{}},
    {gaei::vec3f{1, 0, 0}, gaei::color{}},
    {gaei::vec3f{1, 1, 10}, gaei::color{}},
    {gaei::vec3f{1, 2, 10}, gaei::color{}},
    {gaei::vec3f{1, 3, 20}, gaei::color{}},
    {gaei::vec3f{2, 0, 0}, gaei::color{}},
    {gaei::vec3f{2, 1, 0}, gaei::color{}},
    {gaei::vec3f{2, 2, 0}, gaei::color{}},
    {gaei::vec3f{2, 3, 20}, gaei::color{}}
};
constexpr unsigned res[] = {
    3,0,0,2,
    0,1,1,2,
    3,0,0,2
};

}

OUCHI_TEST_CASE(test_ssi)
{
    gaei::surface_structure_isolate ssi;
    std::vector<gaei::vertex<>> vvec(vs, vs+12);
    ssi(vvec);
    unsigned labels[4] = {};
    for (auto i = 0u; i < vvec.size(); ++i) {
        labels[res[i]] = vvec.at(i).color.value();
    }
    for (auto i = 0u; i < vvec.size(); ++i) {
        OUCHI_CHECK_EQUAL(vvec.at(i).color.value(), labels[res[i]]);
    }
    for (auto i = 0; i < sizeof(labels)/sizeof(*labels); ++i) {
        for (auto j = i + 1; j < sizeof(labels)/sizeof(*labels); ++j) {
            OUCHI_CHECK_TRUE(labels[i] != labels[j]);
        }
    }
}
