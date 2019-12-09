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
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [f = vs.front().position, mt](vertex<>& v) mutable
    {
        v.position.x() -= f.x(); v.position.y() -= f.y();
        v.position.x() = 32 * v.position.x() + mt()%16; v.position.y() = 32 * v.position.y();
    });
}
template<class ExecutionPolicy = std::execution::sequenced_policy>
void inv_normalize(std::vector<vertex<>>& vs)
{
    std::mt19937 mt;
    std::for_each(ExecutionPolicy{}, vs.begin(), vs.end(),
                  [f = vs.front().position, mt](vertex<>& v) mutable
    {
        v.position.x() -= mt()%16; v.position.y() -= 0;
        v.position.x() /= 32; v.position.y() /= 32;
    });
}

}
