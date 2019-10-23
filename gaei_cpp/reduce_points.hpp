#pragma once
#include <vector>
#include <algorithm>
#include "ouchilib/thread/thread-pool.hpp"
#include "vertex.hpp"
#include "surface_structure_isolate.hpp"

namespace gaei {

std::vector<size_t> count_label(size_t label_size, const std::vector<vertex<>>& vs)
{
    std::vector<size_t> lc(label_size, 0);
    for (auto&& i : vs) {
        ++lc[color_to_idx(i.color.value())];
    }
    return std::move(lc);
}

void remove_trivial_surface(size_t label_size, std::vector<vertex<>>& vs)
{
    using ssi = surface_structure_isolate;
    auto lc = count_label(label_size, vs);
    auto max = *std::max_element(lc.begin(), lc.end());
    vs.erase(std::remove_if(vs.begin(), vs.end(),
                            [max](const vertex<>& v) { return v.color.value() == max; }),
             vs.end());
}

}
