﻿#include "color.hpp"
#include "ouchitest.hpp"

OUCHI_TEST_CASE(test_color_set)
{
    gaei::color c = gaei::argb(255, 255, 255, 255);
    OUCHI_CHECK_EQUAL(c, gaei::colors::white);
    c.a(128);
    OUCHI_CHECK_EQUAL(c.a(), 128);
    OUCHI_CHECK_EQUAL(c.r(), 255);
    OUCHI_CHECK_EQUAL(c.g(), 255);
    OUCHI_CHECK_EQUAL(c.b(), 255);
    c.r(14);
    OUCHI_CHECK_EQUAL(c.r(), 14);
}