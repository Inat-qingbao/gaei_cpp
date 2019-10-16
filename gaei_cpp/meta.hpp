#pragma once
#include <type_traits>

namespace gaei {
inline namespace detail {

#define GAEI_IS_OPERATABLE(name, op)\
template<class T, class = void>\
struct name : std::false_type {};\
template<class T>\
struct name <T, std::void_t<decltype(std::declval<T&>() op std::declval<T&>())>>\
    : std::true_type{};\
template<class T>\
constexpr bool name ## _v = name <T>:: value;

GAEI_IS_OPERATABLE(is_addable, +)

GAEI_IS_OPERATABLE(is_multipliable, *)

GAEI_IS_OPERATABLE(is_subtractable, -)

GAEI_IS_OPERATABLE(is_divisible, /)

#undef GAEI_IS_OPERATABLE

#define GAEI_HAS_MEMBER(member_name)\
template<class T, class = void>\
struct has_member_ ## member_name : std::false_type {};\
\
template<class T>\
struct has_member_ ## member_name<T, std::void_t<decltype(&(T::member_name))>> : std::true_type {};\
template<class T>\
inline constexpr bool has_member_ ## member_name ## _v = has_member_ ## member_name <T>::value;

GAEI_HAS_MEMBER(write)

#undef GAEI_HAS_MEMBER

#define GAEI_HAS_MEMBER_TYPE(type_name)\
template<class T, class = void>\
struct has_ ## type_name : std::false_type {};\
template<class T>\
struct has_ ## type_name <T, std::void_t<typename T::type_name>> : std::true_type {};\
template<class T>\
constexpr bool has_ ## type_name ## _v = has_ ## type_name <T>::value;

GAEI_HAS_MEMBER_TYPE(value_type)

#undef GAEI_HAS_MEMBER_TYPE

// because we use c++17, we cannot use `std::remove_cvref` that will be available from c++20.
template<class T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;


}   // namespace detail
}   // namespace gaei
