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
    {
        gaei::vec3f v1{ 1, 1, 1 };
        gaei::vec2f v2{ 1,2 };
        gaei::vec2f v3{ 1,1 };
        gaei::vec2f v4{ 1,0 };
        OUCHI_CHECK_TRUE(v1 < v2);
        OUCHI_CHECK_EQUAL(v1, v3);
        OUCHI_CHECK_TRUE(v1 > v4);
        OUCHI_CHECK_EQUAL(v1, (gaei::vec2f{ 1, 1 }));
    }
}

OUCHI_TEST_CASE(test_vector_operator)
{
    gaei::vec3f v{ 1, 1, 1 };
    gaei::vec2f v2{ 9, 9 };
    OUCHI_CHECK_EQUAL(v + v2, (gaei::vec3f{ 10, 10, 1 }));
    OUCHI_CHECK_EQUAL(v + (gaei::vec3f{ 1,1,1 }), (gaei::vec3f{ 2,2,2 }));
    OUCHI_CHECK_EQUAL(v - (gaei::vec3f{ 1,1,1 }), (gaei::vec3f::zero()));

    OUCHI_CHECK_EQUAL(3 * v, (gaei::vec3f{ 3,3,3 }));
    OUCHI_CHECK_EQUAL(v * 3, (gaei::vec3f{ 3,3,3 }));
    OUCHI_CHECK_EQUAL(v / 4, (gaei::vec3f{ 0.25, 0.25, 0.25 }));
}
