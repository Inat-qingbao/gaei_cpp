﻿#pragma once
#include <cstdint>
#include <tuple>
#include <algorithm>

namespace gaei {

class color {
    std::uint32_t value_;
    bool is_valid_;
public:
    constexpr color(std::uint8_t red,
                    std::uint8_t green,
                    std::uint8_t blue,
                    std::uint8_t alpha = 0xFF) noexcept
        : value_(static_cast<std::uint32_t>(alpha) << 24 |
                 static_cast<std::uint32_t>(red) << 16 |
                 static_cast<std::uint32_t>(green) << 8 |
                 static_cast<std::uint32_t>(blue))
        , is_valid_{ true }
    {}
    constexpr color(std::uint32_t value) noexcept
        : value_{ value }
        , is_valid_{ true }
    {}
    constexpr color() noexcept
        : value_{}
        , is_valid_{ false }
    {}

    [[nodiscard]]
    constexpr explicit operator bool() const noexcept { return is_valid_; }
    [[nodiscard]]
    constexpr bool is_valid() const noexcept { return is_valid_; }
    [[nodiscard]]
    constexpr std::uint32_t value() const noexcept { return value_; }
    [[nodiscard]]
    constexpr unsigned int a() const noexcept { return 0xFFu & static_cast<unsigned int>(value_ >> 24); }
    [[nodiscard]]
    constexpr unsigned int r() const noexcept { return 0xFFu & static_cast<unsigned int>(value_ >> 16); }
    [[nodiscard]]
    constexpr unsigned int g() const noexcept { return 0xFFu & static_cast<unsigned int>(value_ >> 8); }
    [[nodiscard]]
    constexpr unsigned int b() const noexcept { return 0xFFu & static_cast<unsigned int>(value_ >> 0); }
    [[nodiscard]]
    constexpr float af() const noexcept { return static_cast<float>(a() / 255.0f); }
    [[nodiscard]]
    constexpr float rf() const noexcept { return static_cast<float>(r() / 255.0f); }
    [[nodiscard]]
    constexpr float gf() const noexcept { return static_cast<float>(g() / 255.0f); }
    [[nodiscard]]
    constexpr float bf() const noexcept { return static_cast<float>(b() / 255.0f); }

    void validate(bool is_valid = true) noexcept { is_valid_ = is_valid; }

    void a(std::uint8_t alpha) noexcept
    {
        is_valid_ = true;
        value_ &= ~(0xFFul << 24);
        value_ |= static_cast<std::uint32_t>(alpha) << 24;
    }
    void r(std::uint8_t red) noexcept
    {
        is_valid_ = true;
        value_ &= ~(0xFFul << 16);
        value_ |= static_cast<std::uint32_t>(red) << 16;
    }
    void g(std::uint8_t green) noexcept
    {
        is_valid_ = true;
        value_ &= ~(0xFFul << 8);
        value_ |= static_cast<std::uint32_t>(green) << 8;
    }
    void b(std::uint8_t blue) noexcept
    {
        is_valid_ = true;
        value_ &= ~0xFFul;
        value_ |= static_cast<std::uint32_t>(blue);
    }

    [[nodiscard]]
    friend constexpr bool operator==(color lhs, color rhs) noexcept
    {
        //       同じ値なら                    ともにfalseなら
        return !(lhs.value_ ^ rhs.value_) || !(lhs.is_valid_ | rhs.is_valid_);
    }
    [[nodiscard]]
    friend constexpr bool operator!=(color lhs, color rhs) noexcept
    {
        return !(lhs == rhs);
    }
    [[nodiscard]]
    friend constexpr color operator+(color lhs, color rhs) noexcept
    {
        return color(static_cast<std::uint8_t>(std::min(255u, lhs.r() + rhs.r())),
                     static_cast<std::uint8_t>(std::min(255u, lhs.g() + rhs.g())),
                     static_cast<std::uint8_t>(std::min(255u, lhs.b() + rhs.b())),
                     static_cast<std::uint8_t>(std::min(255u, lhs.a() + rhs.a())));
    }
    [[nodiscard]]
    friend constexpr color operator*(color lhs, color rhs) noexcept
    {
        const auto [a, r, g, b] = std::make_tuple(lhs.af() * rhs.af(), lhs.rf() * rhs.rf(),
                                                  lhs.gf() * rhs.gf(), lhs.bf() * rhs.bf());
        return color{ static_cast<std::uint8_t>(a * 255),
                      static_cast<std::uint8_t>(r * 255),
                      static_cast<std::uint8_t>(g * 255),
                      static_cast<std::uint8_t>(b * 255) };
    }
};

[[nodiscard]]
constexpr color rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
{
    return color(r, g, b);
}
[[nodiscard]]
constexpr color argb(std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
{
    return color(r, g, b, a);
}

namespace colors {
inline constexpr color none = color{};
inline constexpr color black = rgb(0, 0, 0);
inline constexpr color white = rgb(255, 255, 255);
inline constexpr color red = rgb(255, 0, 0);
inline constexpr color green = rgb(0, 255, 0);
inline constexpr color blue = rgb(0, 0, 255);
} // namespace colors

}
