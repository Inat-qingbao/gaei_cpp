#pragma once
#include <algorithm>
#include <execution>
#include <vector>

#include <random>

#include "vertex.hpp"
#include "surface_structure_isolate.hpp"

namespace gaei {

template<class ExecutionPolicy = std::execution::sequenced_policy>
inline void normalize(std::vector<vertex<>>& vs)
{
    std::mt19937 mt;
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [f = vs.front().position, mt](vertex<>& v) mutable
    {
        v.position.x() -= f.x(); v.position.y() -= f.y();
        v.position.x() = 32 * v.position.x() + mt()%16; v.position.y() = 32 * v.position.y();
    });
}
template<class ExecutionPolicy = std::execution::sequenced_policy>
inline void inv_normalize(std::vector<vertex<>>& vs)
{
    std::mt19937 mt;
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [f = vs.front().position, mt](vertex<>& v) mutable
    {
        v.position.x() -= mt()%16; v.position.y() -= 0;
        v.position.x() /= 32; v.position.y() /= 32;
    });
}

template<class ExecutionPolicy = std::execution::sequenced_policy>
inline void simplify_color(std::vector<size_t>& lc, std::vector<vertex<>>& vs)
{
    auto ground = std::distance(lc.cbegin(), std::max_element(lc.cbegin(), lc.cend()));
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [ground](vertex<>& v)
                  {if (idx_to_color(v.color.value()) == ground) v.color = colors::green;
                  else v.color = colors::red; });
    lc.clear();
}

}
