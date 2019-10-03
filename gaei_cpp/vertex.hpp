#pragma once
#include <cstddef>
#include <type_traits>

namespace gaei {

/// <summary>
/// 点を表現します。
/// </summary>
/// <typeparam name="T">点の各次元の型</typeparam>
template<class T, std::size_t Dim>
struct vector {
    static_assert(std::is_arithmetic_v<T>);
    /// <summary>
    /// 点の各次元の座標を格納します。
    /// </summary>
    T coord[Dim];

    /// <summary>
    /// 点が1次元以上である場合のみ有効です。点のx座標への参照を返します。
    /// </summary>
    template<size_t D = Dim, std::enable_if_t<(D >= 1)>* = nullptr>
    [[nodiscard]]
    constexpr T& x() noexcept { return coord[0]; };
    template<size_t D = Dim, std::enable_if_t<(D >= 1)>* = nullptr>
    [[nodiscard]]
    constexpr const T& x() const noexcept { return coord[0]; };
    /// <summary>
    /// 点が2次元以上である場合のみ有効です。点のy座標への参照を返します。
    /// </summary>
    template<size_t D = Dim, std::enable_if_t<(D >= 2)>* = nullptr>
    [[nodiscard]]
    constexpr T& y() noexcept { return coord[1]; };
    template<size_t D = Dim, std::enable_if_t<(D >= 2)>* = nullptr>
    [[nodiscard]]
    constexpr const T& y() const noexcept { return coord[1]; };
    /// <summary>
    /// 点が3次元以上である場合のみ有効です。点のz座標への参照を返します。
    /// </summary>
    template<size_t D = Dim, std::enable_if_t<(D >= 3)>* = nullptr>
    [[nodiscard]]
    constexpr T& z() noexcept { return coord[2]; };
    template<size_t D = Dim, std::enable_if_t<(D >= 3)>* = nullptr>
    [[nodiscard]]
    constexpr const T& z() const noexcept { return coord[2]; };
};

using vec2f = vector<float, 2>;
using vec3f = vector<float, 3>;

template<class PosT, class ColorT>
struct vertex {
    PosT position;
    ColorT color = ColorT{};
};

}
