#pragma once
#include <cmath>
#include <type_traits>
#include "vertex.hpp"

namespace gaei {
template<class T, size_t Dim>
T sqdistance(const vector<T, Dim>& a,
             const vector<T, Dim>& b)
    noexcept
{
    using std::pow, std::sqrt;
    T sum{};
    for (auto i = 0u; i < Dim; ++i) {
        auto c = a.coord[i] - b.coord[i];
        sum += c * c;
    }
    return sum;
}

// ベクトルaとbの距離を求める
// T = ベクトルの各要素の型(floatとかdoubleとかintとか)
// Dim = ベクトルの次元
template<class T, size_t Dim>
T distance(const vector<T, Dim>& a,
           const vector<T, Dim>& b)
    noexcept
{
    return static_cast<T>(sqrt(sqdistance(a, b)));
}

// ベクトルaとbの内積
template<class T, size_t Dim>
T inner_product(const gaei::vector<T, Dim>& a,
                const gaei::vector<T, Dim>& b)
    noexcept
{
    T res{};
    for (auto i = 0u; i < Dim; ++i) {
        res += a.coord[i] * b.coord[i];
    }
    return res;
}

// 3次元ベクトルaとbの外積
template<class T>
gaei::vector<T, 3> cross_product(const gaei::vector<T, 3>& a,
                                 const gaei::vector<T, 3>& b)
    noexcept
{
    gaei::vector<T, 3> c;
    c.coord[0] = a.coord[1] * b.coord[2] - a.coord[2] * b.coord[1];
    c.coord[1] = a.coord[2] * b.coord[0] - a.coord[0] * b.coord[2];
    c.coord[2] = a.coord[0] * b.coord[1] - a.coord[1] * b.coord[0];
    return c;
}

}

namespace ouchi::geometry{

template<class T, size_t Dim>
struct point_traits<gaei::vector<T, Dim>> {
    using type = gaei::vector<T, Dim>;
    using coord_type = T;
    static constexpr size_t dim = Dim;

    static constexpr const T& get(const type& p, size_t d) noexcept { return p.coord[d]; }
    static constexpr T& set(type& p, size_t d, const T& v) noexcept { return p.coord[d] = v; }
    static constexpr type add(const type& p1, const type& p2) noexcept { return p1 + p2; }
    static constexpr type sub(const type& p1, const type& p2) noexcept { return p1 - p2; }
    static constexpr T inner_product(const type& p1, const type& p2) noexcept
    {
        return gaei::inner_product(p1, p2);
    }
    static type mul(const type& v, const T& scalar) noexcept { return v * scalar; }
    static constexpr type zero() noexcept { return type{}; }
    static constexpr T sqdistance(const type& p1, const type& p2) noexcept
    {
        return gaei::sqdistance(p1, p2);
    }
    static constexpr T distance(const type& p1, const type& p2) noexcept
    {
        return gaei::distance(p1, p2);
    }
};

template<class Pt, class Col>
struct point_traits<gaei::vertex<Pt, Col>> {
    using pt = point_traits<Pt>;
    using type = gaei::vertex<Pt, Col>;
    using coord_type = typename pt::coord_type;
    static constexpr size_t dim = 2;

    static constexpr const coord_type& get(const type& p, size_t d) noexcept { return pt::get(p.position, d); }
    static constexpr coord_type& set(type& p, size_t d, const coord_type& v) noexcept { return pt::set(p.position, d, v); }
    static constexpr type add(const type& p1, const type& p2) noexcept { return { pt::add(p1.position, p2.position), {} }; }
    static constexpr type sub(const type& p1, const type& p2) noexcept { return { pt::sub(p1.position, p2.position), {} }; }
    static constexpr coord_type inner_product(const type& p1, const type& p2) noexcept
    {
        return gaei::inner_product(p1.position, p2.position);
    }
    static type mul(const type& v, const coord_type& scalar) noexcept { return { v.position * scalar, {} }; }
    static constexpr type zero() noexcept { return type{ {}, {} }; }
    static constexpr coord_type sqdistance(const type& p1, const type& p2) noexcept
    {
        return gaei::sqdistance(p1.position, p2.position);
    }
    static constexpr coord_type distance(const type& p1, const type& p2) noexcept
    {
        return gaei::distance(p1.position, p2.position);
    }
};

}
