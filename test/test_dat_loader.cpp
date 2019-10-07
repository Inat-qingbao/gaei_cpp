#include <sstream>
#include "ouchitest.hpp"
#include "dat_loader.hpp"

OUCHI_TEST_CASE(test_dat_loader)
{
    using namespace std::literals;
    // ダミーの.datを模した正常なデータを用意
    std::stringstream s("  -5967.00  -33278.00    19.00\r\n");
    gaei::dat_loader dl;
    // ダミーデータを頂点集合として取り出す
    auto r = dl.load(s);
    // この時点でrはtrueに評価されて、頂点の数は1, (10, 11, 1)座標の点が含まれているべき
    OUCHI_CHECK_TRUE(r);
    auto v = r.unwrap();
    OUCHI_CHECK_EQUAL(v.front().position.x(), -5967.00);
    OUCHI_CHECK_EQUAL(v.front().position.y(), -33278.00);
    OUCHI_CHECK_EQUAL(v.front().position.z(), 19.0);
    OUCHI_CHECK_EQUAL(v.size(), 1);
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
