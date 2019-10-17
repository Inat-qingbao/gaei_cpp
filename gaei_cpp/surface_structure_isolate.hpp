#pragma once
#include <functional>
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
    static constexpr std::uint32_t border = 1u << 31;
    surface_structure_isolate(float diff = 4)
        : not_visited_{}
        , vertexes_{ nullptr }
        , diff_{ diff }
        , color_{ 0 }
    {}
    /// <remarks>
    /// vertexesは変更されないが、要素を変更するためconst参照ではない。
    /// </remarks>
    auto operator() (std::vector<vertex<>>& vertexes)
    {
        color_ = 0;
        vertexes_ = &vertexes;
        for (auto it = vertexes.begin(); it != vertexes.end(); ++it) {
            not_visited_.insert(it);
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
            vec2f nv = tv + dv;
            auto [nitr, unused] = std::equal_range(vertexes_->begin(), vertexes_->end(), nv,
                                                   overloaded{
                                                   [](const vertex<>& vv, const vec2f& v) {return vv.position < v; },
                                                   [](const vec2f& v, const vertex<>& vv) {return v < vv.position; }
                                                   });

            // nvの位置に要素がないか、すでに訪問済みならば次の探索候補を見る
            if (nitr == vertexes_->end() || nitr->color.is_valid()) continue;
            // nvの位置の要素に別のラベルを付けるべきなら境界印をつけて次の探索候補を見る
            if (std::abs(nitr->position.z() - t->position.z()) > diff_) {
                t->color = color{ label | border };
                continue;
            }
            visit(nitr, label);
        }
    }
};

}
