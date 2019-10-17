#pragma once
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <queue>

#include "vertex.hpp"
#include "color.hpp"
#include "ouchilib/result/result.hpp"

namespace gaei {

class surface_structure_isolate {
public:
    /// <remarks>
    /// vertexesは変更されないが、要素を変更するためconst参照ではない。
    /// </remarks>
    surface_structure_isolate(float diff = 4)
        : not_visited_{}
        , vertexes_{ nullptr }
        , diff_{ diff }
        , color_{ 0 }
    {}
    auto operator() (std::vector<vertex<>>& vertexes)
    {
        color_ = 0;
        vertexes_ = &vertexes;
        for (auto it = vertexes.begin(); it != vertexes.end(); ++it) {
            not_visited_.insert(vertexes.begin());
        }
        while (not_visited_.size()) {
            visit(*not_visited_.begin(), color_++);
        }
        vertexes_ = nullptr;
        return color_;
    }
private:
    std::set<std::vector<vertex<>>::iterator> not_visited_;
    std::vector<vertex<>>* vertexes_;
    float diff_;
    std::uint32_t color_;
    static constexpr std::uint32_t border = 1u << 31;

    void visit(std::vector<vertex<>>::iterator t, std::uint32_t label) noexcept
    {
        constexpr vec2f d[4] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
        vec2f tv{ t->position.x(), t->position.y() };
        // 訪問済みのしるしをつける
        t->color = color{ label };
        // 未訪問リストから消す
        not_visited_.erase(t);
        // 探索は4方向に伸びていく
        for (auto&& dv : d) {
            auto nv = tv + dv;
            auto nitr = std::binary_search(vertexes_->begin(), vertexes_->end(), nv);
            if (nitr == vertexes_->end()) continue;
            if (std::abs(nitr->position.z() - t->position.z()) > diff_) {
                t->color = color{ label | border };
                continue;
            }
            visit(nitr, label);
        }
    }
};

}
