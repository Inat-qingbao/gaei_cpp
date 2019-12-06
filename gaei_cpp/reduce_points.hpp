#pragma once
#include <vector>
#include <algorithm>

#include <iostream>

#include "ouchilib/thread/thread-pool.hpp"
#include "vertex.hpp"
#include "surface_structure_isolate.hpp"

namespace gaei {

inline std::vector<size_t> count_label(size_t label_size, const std::vector<vertex<>>& vs)
{
    std::vector<size_t> lc(label_size, 0);
    for (auto&& i : vs) {
        ++lc[color_to_idx(i.color.value())];
    }
    return std::move(lc);
}

inline void remove_trivial_surface(const std::vector<size_t>& lc, std::vector<vertex<>>& vs)
{
    using ssi = surface_structure_isolate;
    auto max = std::distance(lc.cbegin(), std::max_element(lc.cbegin(), lc.cend()));
    vs.erase(std::remove_if(vs.begin(), vs.end(),
                            [max](const vertex<>& v) { return v.color.value() == max; }),
             vs.end());
}

inline void remove_minor_labels(std::vector<size_t> lc, std::vector<vertex<>>& vs, size_t threshold = 5) noexcept
{
    vs.erase(std::remove_if(vs.begin(), vs.end(),
                            [threshold, &lc](const vertex<>& v) { return lc[color_to_idx(v.color.value())] < threshold; }),
             vs.end());
}

inline void remove_error_point(std::vector<vertex<>>& vs) noexcept
{
//-9999.99
    auto b = vs.size();
    vs.erase(std::remove_if(vs.begin(), vs.end(),
                            [](const vertex<>& v) { return v.position.z() < -9000; }),
             vs.end());
    std::cout << "removed error:" << b - vs.size() << '\n';
}

inline void thinout(std::vector<vertex<>>& vs) noexcept
{
    std::sort(vs.begin(), vs.end(),
              [](auto&& a, auto&& b) {return a.position < b.position; });
    vs.erase(std::remove_if(vs.begin(), vs.end(),
                            [idx = 0u](const vertex<>& v) mutable {return v.color.a() != 0 &&((int)v.position.x() % 4 || (int)v.position.y() % 4); }),
             vs.end());
}

}
