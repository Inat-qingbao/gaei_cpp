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
ouchi::result::result<std::size_t, std::errc>
to_vrml(color val, char* first, char* last) noexcept
{
    std::to_chars_result r;
    assert(first < last);
    r = std::to_chars(first, last, val.rf());
    *(r.ptr) = ' ';
    ++(r.ptr);
    assert(r.ptr < last);
    r = std::to_chars(r.ptr, last, val.gf());
    *(r.ptr) = ' ';
    ++(r.ptr);
    assert(r.ptr < last);
    r = std::to_chars(r.ptr, last, val.bf());
    *(r.ptr) = 0;
    if (r.ec != std::errc{}) return ouchi::result::err(r.ec);
    return ouchi::result::ok(first - r.ptr - 1);
}

template<size_t InternalBufferLength = 64>
ouchi::result::result<std::size_t, std::errc>
to_vrml(color& val, std::string& dest)
{
    char buffer[InternalBufferLength] = {};
    auto r = to_vrml(val, buffer, buffer + InternalBufferLength);
    if(r) dest.append(buffer);
    return r;
}

ouchi::result::result<std::size_t, std::errc>
to_vrml(const vec3f& val, char* first, char* last) noexcept
{
    std::to_chars_result r;
    assert(first < last);
    r = std::to_chars(first, last, val.x());
    *(r.ptr) = ' ';
    ++(r.ptr);
    assert(r.ptr < last);
    r = std::to_chars(r.ptr, last, val.y());
    *(r.ptr) = ' ';
    ++(r.ptr);
    assert(r.ptr < last);
    r = std::to_chars(r.ptr, last, val.z());
    *(r.ptr) = 0;
    if (r.ec != std::errc{}) return ouchi::result::err(r.ec);
    return ouchi::result::ok(first - r.ptr - 1);
}

template<size_t InternalBufferLength = 64>
ouchi::result::result<std::size_t, std::errc>
to_vrml(const vec3f& val, std::string& dest)
{
    char buffer[InternalBufferLength] = {};
    auto r = to_vrml(val, buffer, buffer + InternalBufferLength);
    if(r) dest.append(buffer);
    return r;
}

}
