#include <sstream>
#include <string_view>
#include "ouchitest.hpp"
#include "vrml_writer.hpp"

namespace {

constexpr std::string_view correct_indexed_face_set =
"geometry IndexedFaceSet{\n"
"coord Coordinate{point[0 0 0 \n"
"1 1 1 \n"
"]\n"
"}\n"
"}\n";

gaei::vertex<gaei::vec3f, gaei::color> vs[] = {
    {{0,0,0}, gaei::colors::none},
    {{1,1,1}, gaei::colors::none}
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

OUCHI_TEST_CASE(test_point_set)
{
    gaei::vrml::vrml_writer vw;
    gaei::vrml::shape<gaei::vrml::point_set, gaei::vrml::appearance<>> ps;
    ps.geometry().points.assign(std::begin(vs), std::end(vs));
    vw.push(ps);
    vw.write(".\\pointset.wrl");
    // テストを書くのが非常にしんどいので表示して確認してくれ！！！！
}

OUCHI_TEST_CASE(test_transform)
{

    gaei::vrml::vrml_writer vw;
    gaei::vrml::shape<gaei::vrml::box, gaei::vrml::appearance<>> box;
    gaei::vrml::transform t;
    t.children.push_back(std::make_unique<decltype(box)>(box));
    t.translation = gaei::vec3f{ 1,3,5 };
    vw.push(std::move(t));
    vw.write(".\\boxtransform.wrl");
    // テストを書くのが非常にしんどいので表示して確認してくれ！！！！
}
