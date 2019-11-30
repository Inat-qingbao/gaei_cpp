#pragma once
#include <algorithm>
#include <execution>
#include <vector>

#include <random>

#include "vertex.hpp"

namespace gaei {

template<class ExecutionPolicy = std::execution::sequenced_policy>
void normalize(std::vector<vertex<>>& vs)
{
    std::mt19937 mt;
    std::uniform_real_distribution<double> di(30.0f, 30.0f);
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [f = vs.front().position, mt, di](vertex<>& v) mutable
    {v.position.x() -= f.x(); v.position.y() -= f.y(); });
}

}
