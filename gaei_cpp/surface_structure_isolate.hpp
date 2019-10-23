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
#include "ouchilib/crypto/algorithm/aes.hpp"

namespace gaei {

std::uint32_t idx_to_color(unsigned int idx) noexcept
{
    return 0xFFFFFF & ouchi::crypto::aes128::subword(idx);
}

class surface_structure_isolate {
public:
    static constexpr std::uint32_t border = 1u << 31;
    surface_structure_isolate(float diff = 4)
        : not_visited_{}
        , diff_{ diff }
    {}
    /// <remarks>
    /// vertexesは変更されないが、要素を変更するためconst参照ではない。
    /// </remarks>
    template<class C>
    auto operator() (C&& vertexes)
    {
        std::uint32_t color = 0;
        for (auto it = vertexes.begin(); it != vertexes.end(); ++it) {
            not_visited_.insert(it);
        }
        while (not_visited_.size()) {
            visit(*not_visited_.begin(), vertexes.begin(), vertexes.end(), idx_to_color(color++));
        }
        return color;
    }
private:
    std::set<std::vector<vertex<>>::iterator> not_visited_;
    float diff_;

    template<class Itr, std::enable_if_t<std::is_same_v<vertex<>, typename std::iterator_traits<Itr>::value_type>>* = nullptr>
    void visit(Itr t, Itr first, Itr last, std::uint32_t label) noexcept
    {
        constexpr vec2f d[4] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
        std::queue<Itr> queue;
        queue.push(t);
        while (queue.size()) {
            auto ot = queue.front();
            queue.pop();
            // 訪問済みのしるしをつける
            ot->color = color{ label };
            // 未訪問リストから消す
            not_visited_.erase(ot);
            vec2f tv{ ot->position.x(), ot->position.y() };
            // 探索は4方向に伸びていく
            for (auto&& dv : d) {
                vec2f nv = tv + dv;
                auto nitr = std::lower_bound(first, last, nv, 
                                           overloaded{
                                           [](const vertex<>& vv, const vec2f& v) {return vv.position < v; },
                                           [](const vec2f& v, const vertex<>& vv) {return v < vv.position; }
                                           });
                // nvの位置に要素がないならば境界印を付けて次の探索候補を見る
                // nvの位置の要素に別のラベルを付けるべきなら境界印をつけて次の探索候補を見る
                if (nitr->position != nv || std::abs(nitr->position.z() - ot->position.z()) > diff_) {
                    ot->color = color{ label | border };
                    continue;
                }
                // すでに訪問済みならば次の探索候補を見る
                if (nitr->color.is_valid()) continue;
                nitr->color.validate();
                queue.push(nitr);
            }
        }
    }
};

}
