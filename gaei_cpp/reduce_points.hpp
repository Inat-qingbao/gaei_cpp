#pragma once
#include <vector>
#include <algorithm>
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
    vs.erase(std::remove_if(vs.begin(), vs.end(),
                            [](const vertex<>& v) { return v.position.z() < -9000; }),
             vs.end());
}


}
