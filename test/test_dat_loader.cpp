#include <sstream>
#include "ouchitest.hpp"
#include "dat_loader.hpp"

OUCHI_TEST_CASE(test_dat_loader){
    using namespace std::literals;
    std::stringstream s("      10.0       11.0      1.0\r\n");
    gaei::dat_loader dl;
    auto r = dl.load(s);

    OUCHI_CHECK_TRUE(r);
    auto v = r.unwrap();
    OUCHI_CHECK_EQUAL(v.front().position.x(), 10.0);
    OUCHI_CHECK_EQUAL(v.front().position.y(), 11.0);
    OUCHI_CHECK_EQUAL(v.front().position.z(), 1.0);
}

OUCHI_TEST_CASE(test_dat_loader_vector_err)
{
    using namespace std::literals;
    std::stringstream s("      10.0       11.0         \r\n");
    gaei::dat_loader dl;
    auto r = dl.load(s);
    // result must be err
    OUCHI_CHECK_TRUE(!r);
}

OUCHI_TEST_CASE(test_dat_loader_translate_err)
{
    using namespace std::literals;
    std::stringstream s("      10.0       11.0      aaa\r\n");
    gaei::dat_loader dl;
    auto r = dl.load(s);
    // result must be err
    OUCHI_CHECK_TRUE(!r);
}
