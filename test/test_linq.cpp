#include "linq.hpp"
#include "ouchitest.hpp"
#include <vector>
#include <list>

static_assert(gaei::is_container_v<std::vector<int>>);
static_assert(gaei::is_container_v<int[10]>);

OUCHI_TEST_CASE(test_meta_container)
{
    using V = std::vector<int>;
    std::vector<int> v;
    OUCHI_CHECK_EQUAL(gaei::container_traits<V>::begin(v), v.begin());
    using A = int[10];
    int a[10] = {};
    OUCHI_CHECK_EQUAL(gaei::container_traits<A>::begin(a), a);
    OUCHI_CHECK_EQUAL(gaei::container_traits<A>::begin(std::as_const(a)), a);
}
