#pragma once
#include <cstddef>
#include <type_traits>
#include <algorithm>
#include "color.hpp"
#include "ouchilib/utl/multiitr.hpp"
#include "meta.hpp"

namespace gaei {

/// <summary>
/// 点を表現します。
/// </summary>
/// <typeparam name="T">点の各次元の型</typeparam>
template<class T, std::size_t Dim>
struct vector {
    // 四則演算が可能か？
    static_assert(detail::is_addable_v<T> &&
                  detail::is_multipliable_v<T> &&
                  detail::is_subtractable_v<T> &&
                  detail::is_divisible_v<T>);
    // 次元は0より大きいか？
    static_assert(Dim > 0);

    using value_type = T;
    static constexpr size_t dimension = Dim;

    /// <summary>
    /// 点の各次元の座標を格納します。
    /// </summary>
    T coord[Dim];

    /// <summary>
    /// プログラムがwell-formedであるとき常に有効です。点のx座標への参照を返します。
    /// </summary>
    template<size_t D = Dim, std::enable_if_t<(D >= 1)>* = nullptr>
    [[nodiscard]]
    constexpr T& x() noexcept { return coord[0]; };
    template<size_t D = Dim, std::enable_if_t<(D >= 1)>* = nullptr>
    [[nodiscard]]
    constexpr const T& x() const noexcept { return coord[0]; };
    /// <summary>
    /// 点が2次元以上である場合のみ有効です。点のy座標への参照を返します。
    /// 点が2次元未満であるときこのメソッドは実体化に失敗します。
    /// </summary>
    template<size_t D = Dim, std::enable_if_t<(D >= 2)>* = nullptr>
    [[nodiscard]]
    constexpr T& y() noexcept { return coord[1]; };
    template<size_t D = Dim, std::enable_if_t<(D >= 2)>* = nullptr>
    [[nodiscard]]
    constexpr const T& y() const noexcept { return coord[1]; };
    /// <summary>
    /// 点が3次元以上である場合のみ有効です。点のz座標への参照を返します。
    /// 点が3次元未満であるときこのメソッドは実体化に失敗します。
    /// </summary>
    template<size_t D = Dim, std::enable_if_t<(D >= 3)>* = nullptr>
    [[nodiscard]]
    constexpr T& z() noexcept { return coord[2]; };
    template<size_t D = Dim, std::enable_if_t<(D >= 3)>* = nullptr>
    [[nodiscard]]
    constexpr const T& z() const noexcept { return coord[2]; };

    [[nodiscard]]
    static constexpr vector zero() noexcept { return vector{}; }

    /// <summary>
    /// 辞書順に並べたときlhsがrhsより前より来るならばtrue。それ以外はfalse
    /// </summary>
    template<size_t D>
    [[nodiscard]]
    friend constexpr bool operator< (const vector& lhs, const vector<T, D>& rhs) noexcept
    {
        constexpr auto d = std::min(Dim, D);
        for (auto i = 0; i < d; ++i) {
            if (lhs.coord[i] == rhs.coord[i]) continue;
            else return lhs.coord[i] < rhs.coord[i];
        }
        return false;
    }
    template<size_t D>
    [[nodiscard]]
    friend constexpr bool operator> (const vector& lhs, const vector<T, D>& rhs) noexcept
    {
        return rhs < lhs;
    }
    template<size_t D>
    [[nodiscard]]
    friend constexpr bool operator== (const vector& lhs, const vector<T, D>& rhs) noexcept
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }
    template<size_t D>
    [[nodiscard]]
    friend constexpr bool operator!= (const vector& lhs, const vector<T, D>& rhs) noexcept
    {
        return !(lhs == rhs);
    }
    template<size_t D>
    [[nodiscard]]
    friend constexpr bool operator<= (const vector& lhs, const vector<T, D>& rhs) noexcept
    {
        return (lhs < rhs) || (lhs == rhs);
    }
    template<size_t D>
    [[nodiscard]]
    friend constexpr bool operator>= (const vector& lhs, const vector<T, D>& rhs) noexcept
    {
        return (lhs > rhs) || (lhs == rhs);
    }
};

using vec2f = vector<float, 2>;
using vec3f = vector<float, 3>;

template<class PosT = vec3f, class ColorT = color>
struct vertex {
    PosT position = PosT{};
    ColorT color = ColorT{};
};

}
