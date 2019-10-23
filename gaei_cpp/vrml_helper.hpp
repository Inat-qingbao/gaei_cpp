#pragma once
#include <cassert>
#include <system_error>
#include <string>
#include <charconv>
#include "color.hpp"
#include "vertex.hpp"
#include "ouchilib/result/result.hpp"

namespace gaei::vrml {

/// <summary>
/// 指定されたオブジェクトをVRML形式にして出力する
/// </summary>
/// <returns>
/// 成功した場合書き込んだ文字列の長さ。失敗した場合エラーコード
/// </returns>
inline ouchi::result::result<std::size_t, std::errc>
inline to_vrml(color val, char* first, char* last) noexcept
{
    std::to_chars_result r;
    r = std::to_chars(first, last, val.rf());
    *(r.ptr) = ' ';
    ++(r.ptr);
    r = std::to_chars(r.ptr, last, val.gf());
    *(r.ptr) = ' ';
    ++(r.ptr);
    r = std::to_chars(r.ptr, last, val.bf());
    *(r.ptr) = 0;
    if (r.ec != std::errc{}) return ouchi::result::err(r.ec);
    return ouchi::result::ok(first - r.ptr - 1);
}

template<size_t InternalBufferLength = 64>
inline ouchi::result::result<std::size_t, std::errc>
to_vrml(color val, std::string& dest)
{
    char buffer[InternalBufferLength] = {};
    auto r = to_vrml(val, buffer, buffer + InternalBufferLength);
    if(r) dest.append(buffer);
    return r;
}

namespace detail {

template<class T, size_t D, size_t ...S>
inline ouchi::result::result<std::size_t, std::errc>
vec_unfold(const vector<T, D>& val, char* first, char* last, std::index_sequence<S...>) noexcept
{
    std::to_chars_result r;
    r.ptr = first;
    (((r = std::to_chars(r.ptr, last, val.coord[S])), *(r.ptr++) = ' '), ...);
    if (r.ec != std::errc{}) return ouchi::result::err(r.ec);
    return ouchi::result::ok(first - r.ptr - 1);
}

}

template<class T, size_t S>
inline ouchi::result::result<std::size_t, std::errc>
to_vrml(const vector<T, S>& val, char* first, char* last) noexcept
{
    std::to_chars_result r;
    return detail::vec_unfold(val, first, last, std::make_index_sequence<S>{});
}

template<class T, size_t S, size_t InternalBufferLength = 64>
inline ouchi::result::result<std::size_t, std::errc>
to_vrml(const vector<T, S>& val, std::string& dest)
{
    char buffer[InternalBufferLength] = {};
    auto r = to_vrml(val, buffer, buffer + InternalBufferLength);
    if(r) dest.append(buffer);
    return r;
}

}
