#include "ouchitest.hpp"
#include "vertex.hpp"

OUCHI_TEST_CASE(test_vertex_instantiate)
{
    gaei::vec2f v2;
    v2.y() = 0;
    OUCHI_CHECK_EQUAL(v2.coord[1], 0);
}
