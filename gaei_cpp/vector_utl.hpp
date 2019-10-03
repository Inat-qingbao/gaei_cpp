#pragma once
#include <cmath>
#include <type_traits>
#include "vertex.hpp"

namespace gaei{

// ベクトルaとbの距離を求める
// T = ベクトルの各要素の型(floatとかdoubleとかintとか)
// Dim = ベクトルの次元
template<class T, size_t Dim>
T distance(const gaei::vector<T, Dim>& a,
	       const gaei::vector<T, Dim>& b)
    noexcept
{
	T sum{};
    for(auto i = 0u; i < Dim; ++i) {
		sum += std::pow(a.coord[i] - b.coord[i], 2);
    }
	return T{ std::sqrt(sum) };
}

// ベクトルaとbの内積
template<class T, size_t Dim>
T inner_product(const gaei::vector<T, Dim>& a,
	            const gaei::vector<T, Dim>& b)
    noexcept
{
	T res{};
    for(auto i = 0u; i < Dim; ++i) {
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
	c.coord[0] = a.coord[1] * b.coord[2] - a.coord[2] - b.coord[1];
	c.coord[1] = a.coord[2] * b.coord[0] - a.coord[0] - b.coord[2];
	c.coord[2] = a.coord[0] * b.coord[1] - a.coord[1] * b.coord[0];
	return c;
}

}
