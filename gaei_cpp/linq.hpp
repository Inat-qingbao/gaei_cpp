#pragma once
#include <type_traits>
#include <iterator>
#include "meta.hpp"

namespace gaei {

inline namespace detail {

template<class T, class = void>
struct is_container : std::false_type {};
template<class T>
struct is_container<T, std::enable_if_t<std::conjunction_v<
    has_const_iterator<T>, has_iterator<T>,
    has_member_begin<T>, has_member_end<T>,
    has_value_type<T>>, void>>
    : std::true_type {};
template<class T, size_t S>
struct is_container<T[S], void> : std::true_type {};

template<class T>
constexpr bool is_container_v = is_container<T>::value;

template<class T>
struct is_array : std::false_type{};
template<class T, size_t S>
struct is_array<T[S]> : std::true_type {};
template<class T>
constexpr bool is_array_v = is_array<T>::value;

template<class T, class = void>
struct container_traits {};

template<class T>
struct container_traits<T,std::enable_if_t<is_container_v<T> && !is_array_v<T>>> {
    using value_type = typename T::value_type;

    using iterator = typename T::iterator;

    using const_iterator =  typename T::const_iterator;

    static iterator begin(T& t) noexcept(noexcept(std::declval<T&>().begin()))
    {
        return t.begin();
    }
    static const_iterator begin(const T& t) noexcept(noexcept(std::declval<T&>().begin()))
    {
        return t.cbegin();
    }
    static const_iterator cbegin(const T& t) noexcept(noexcept(std::declval<T&>().begin()))
    {
        return t.cbegin();
    }
    static iterator end(T& t) noexcept(noexcept(std::declval<T&>().end()))
    {
        return t.end();
    }
    static const_iterator end(const T& t) noexcept(noexcept(std::declval<T&>().end()))
    {
        return t.cend();
    }
    static const_iterator cend(const T& t) noexcept(noexcept(std::declval<T&>().end()))
    {
        return t.cend();
    }
};

template<class T, size_t S>
struct container_traits<T[S], void> {
    using value_type = T;

    using iterator = T*;

    using const_iterator = const T*;

    static iterator begin(T(&t)[S]) noexcept
    {
        return t;
    }
    static const_iterator begin(const T(&t)[S]) noexcept
    {
        return t;
    }
    static const_iterator cbegin(const T(&t)[S]) noexcept
    {
        return begin(t);
    }
    static iterator end(T(&t)[S]) noexcept(noexcept(std::declval<T&>().end()))
    {
        return t + S;
    }
    static const_iterator end(const T(&t)[S]) noexcept(noexcept(std::declval<T&>().end()))
    {
        return t + S;
    }
    static iterator cend(const T(&t)[S]) noexcept(noexcept(std::declval<T&>().end()))
    {
        return end(t);
    }
};

} // namespace detail

template<class T, std::enable_if_t<is_container_v<remove_cvref_t<T>>>* = nullptr>
class linq {
public:
    using container_type = remove_cvref_t<T>;
    using value_type = typename container_type::value_type;
    struct iterator{};
};

}
