#include "ouchitest.hpp"
#include "vertex.hpp"

OUCHI_TEST_CASE(test_vector_instantiate)
{
    gaei::vec2f v2;
    v2.y() = 0;
    OUCHI_CHECK_EQUAL(v2.coord[1], 0);
}

OUCHI_TEST_CASE(test_vector_order)
{
    {
        gaei::vec2f v1{ 0, 100 };
        gaei::vec2f v2{ 1, 0 };
        OUCHI_CHECK_TRUE(v1 < v2);
        OUCHI_CHECK_TRUE(v2 > v1);
        OUCHI_CHECK_TRUE(v1 != v2);
    }
    {
        gaei::vec2f v1{ 2, 10 };
        gaei::vec2f v2{ 2,20 };
        OUCHI_CHECK_TRUE(v1 < v2);
        OUCHI_CHECK_TRUE(v2 > v1);
        OUCHI_CHECK_TRUE(v1 != v2);
    }
}
