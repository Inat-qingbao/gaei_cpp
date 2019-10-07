#include <sstream>
#include <string_view>
#include "ouchitest.hpp"
#include "vrml_writer.hpp"

namespace {

constexpr std::string_view correct_indexed_face_set =
"geometry IndexedFaceSet{coord Coordinate{point[0 0 0\n"
"1 1 1\n"
"]\n"
"}\n"
"color Color{color[1 1 1\n"
"1 1 1\n"
"]}}\n";

gaei::vertex<gaei::vec3f, gaei::color> vs[] = {
    {{0,0,0}, {255,255,255}},
    {{1,1,1}, {255,255,255}}
};

}

OUCHI_TEST_CASE(test_indexed_face_set)
{
    std::stringstream ss;
    gaei::vrml::indexed_face_set faceset;
    faceset.data().assign(std::begin(vs), std::end(vs));
    faceset.write(ss);
    OUCHI_CHECK_EQUAL(ss.str(), correct_indexed_face_set);
}

OUCHI_TEST_CASE(test_vrml_writer)
{
    gaei::vrml::vrml_writer vw;
    gaei::vrml::shape<gaei::vrml::box, gaei::vrml::appearance<>> box;
    vw.push(box);
    vw.write(".\\box.wrl");
    // テストを書くのが非常にしんどいので表示して確認してくれ！！！！
}
