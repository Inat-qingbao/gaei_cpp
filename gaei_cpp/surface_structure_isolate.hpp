#pragma once
#include <functional>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <set>
#include <deque>
#include <queue>

#include "ouchilib/thread/thread-pool.hpp"
#include "vertex.hpp"
#include "color.hpp"

namespace gaei {

// MSVCのabs関数がinline指定されていないため。
template<class T>
constexpr auto abs(T a) noexcept
-> std::enable_if_t<std::is_arithmetic_v<T>, T>
{
    return a < 0 ? -a : a;
}

constexpr std::uint32_t idx_to_color(unsigned int idx) noexcept
{
    return 0xFFFFFF & idx;
}

constexpr unsigned int color_to_idx(std::uint32_t color) noexcept
{
    return 0xFFFFFF & color;
}

class surface_structure_isolate {
public:
    static constexpr std::uint32_t border = 1u << 31;
    surface_structure_isolate(float diff = 4)
        : not_visited_{}
        , diff_{ diff }
        , tp_{ 4 }
    {}
    /// <remarks>
    /// vertexesは変更されないが、要素を変更するためconst参照ではない。
    /// </remarks>
    template<class C>
    auto operator()(C&& vertexes)
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
    std::deque<std::vector<vertex<>>::iterator> queue_;
    float diff_;
    ouchi::thread::thread_pool tp_;
    static constexpr vec2f d[4] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0} };


    template<class Itr, std::enable_if_t<std::is_same_v<vertex<>, typename std::iterator_traits<Itr>::value_type>>* = nullptr>
    void visit(Itr t, Itr first, Itr last, std::uint32_t label)
    {
        queue_.push_back(t);
        auto look = [this, label, first, last](auto&& nitr, auto&& ot, auto&& nv) {
            // nvの位置に要素がないならば境界印を付けて次の探索候補を見る
            // nvの位置の要素に別のラベルを付けるべきなら境界印をつけて次の探索候補を見る
            if (nitr->position != nv) return;
            if (abs(nitr->position.z() - ot->position.z()) > diff_) {
                ot->color = color{ label | border };
                return;
            }
            // すでに訪問済みならば次の探索候補を見る
            if (nitr->color.is_valid()) return;
            nitr->color.validate();
            queue_.push_back(nitr);
        };
        while (queue_.size()) {
            auto ot = queue_.front();
            queue_.pop_front();
            // 訪問済みのしるしをつける
            ot->color = color{ label };
            // 未訪問リストから消す
            not_visited_.erase(ot);
            const vec2f tv{ ot->position.x(), ot->position.y() };
            // 探索は4方向に伸びていく

            // 最初の2方向は(メモリ上で)前後の要素
            for (auto i = 0; i < 2; ++i) {
                const vec2f nv = tv + d[i];
                if ((!i && ot == first) || (i && ot == last)) continue;
                look((ot + ((i << 1) - 1)), ot, nv);
            }

            for (auto i = 2u; i < 4; ++i) {
                const vec2f nv = tv + d[i];
                const auto nitr = std::lower_bound(first, last, nv,
                                                   [](const vertex<>& vv, const vec2f& v) {return vv.position < v; });
                look(nitr, ot, nv);
            }
        }
    }
};

}
