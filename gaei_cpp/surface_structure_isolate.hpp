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
#include <unordered_set>
#include <unordered_map>

#include <chrono>

#include "vertex.hpp"
#include "color.hpp"
#include "ouchilib/crypto/common.hpp"

namespace std {

template<>
struct hash<gaei::vec2f> {
    typedef size_t result_type;
    typedef gaei::vec2f argument_type;

    size_t operator()(const gaei::vec2f& v) const noexcept
    {
        constexpr auto x_shift = 12u;
        constexpr auto y_shift = 0u;
        return
            ((size_t)(int)v.x() << x_shift) ^
            ((size_t)(int)v.y() << y_shift);
    }
};
}

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
    {}
    /// <remarks>
    /// vertexesは変更されないが、要素を変更するためconst参照ではない。
    /// </remarks>
    template<class C>
    auto operator()(C&& vertexes)
    {
        std::uint32_t color = 0;
        work_space_.reserve(vertexes.size());
        not_visited_.reserve(vertexes.size());
        for (auto it = vertexes.begin(); it != vertexes.end(); ++it) {
            work_space_.insert_or_assign({it->position.x(), it->position.y()}, *it);
            not_visited_.insert({ it->position.x(), it->position.y() });
        }
        while (not_visited_.size()) {
            visit(*not_visited_.begin(), idx_to_color(color++));
        }
        for (auto& v : vertexes) {
            v.color = work_space_.find({ v.position.x(), v.position.y() })->second.color;
        }
        return color;
    }
private:
    std::unordered_set<vec2f> not_visited_;
    std::unordered_map<vec2f, vertex<>> work_space_;
    std::deque<vec2f> queue_;
    float diff_;
    static constexpr vec2f d[4] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0} };


    void visit(vec2f t, std::uint32_t label)
    {
        queue_.push_back(t);
        auto look = [this, label](auto&& nv, auto&& ot, auto&& cpos, auto&& invalid) {
            // nvの位置に要素がないならば境界印を付けて次の探索候補を見る
            // nvの位置の要素に別のラベルを付けるべきなら境界印をつけて次の探索候補を見る
            if (nv == invalid || abs(nv->second.position.z() - ot->second.position.z()) > diff_) {
                ot->second.color = color{ label | border };
                return;
            }
            // すでに訪問済みならば次の探索候補を見る
            if (nv->second.color.is_valid()) return;
            nv->second.color.validate();
            queue_.push_back(cpos);
        };
        while (queue_.size()) {
            auto ov = queue_.front();
            auto ot = work_space_.find(ov);
            queue_.pop_front();
            // 訪問済みのしるしをつける
            ot->second.color = color{ label };
            // 未訪問リストから消す
            not_visited_.erase(ov);
            const vec2f tv{ ot->second.position.x(), ot->second.position.y() };
            // 探索は4方向に伸びていく

            for (auto i = 0u; i < 4; ++i) {
                const vec2f cpos = tv + d[i];
                auto nitr = work_space_.find(cpos);
                look(nitr, ot, cpos, work_space_.end());
            }
        }
    }
};

}
