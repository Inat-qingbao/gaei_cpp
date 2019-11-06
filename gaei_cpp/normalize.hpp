#pragma once
#include <algorithm>
#include <execution>
#include <vector>
#include "vertex.hpp"

namespace gaei {

template<class ExecutionPolicy = std::execution::sequenced_policy>
void normalize(std::vector<vertex<>>& vs)
{
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [f = vs.front().position](vertex<>& v) {v.position.x() -= f.x(); v.position.y() -= f.y(); });
}

}
