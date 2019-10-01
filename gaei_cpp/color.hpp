﻿#pragma once
#include <cstdint>

namespace gaei {

struct color {
    constexpr color(std::uint8_t red,
                    std::uint8_t green,
                    std::uint8_t blue,
                    std::uint8_t alpha = 0xFF)
        : value(
            static_cast<std::uint32_t>(alpha) << 24 |
            static_cast<std::uint32_t>(red) << 16 |
            static_cast<std::uint32_t>(green) << 8 |
            static_cast<std::uint32_t>(blue)
        )
    {}
    color() = default;

    std::uint32_t value;

    constexpr std::uint8_t a() const noexcept { return static_cast<std::uint8_t>(value >> 24); }
    constexpr std::uint8_t r() const noexcept { return static_cast<std::uint8_t>(value >> 16); }
    constexpr std::uint8_t g() const noexcept { return static_cast<std::uint8_t>(value >> 8); }
    constexpr std::uint8_t b() const noexcept { return static_cast<std::uint8_t>(value >> 0); }

    void a(std::uint8_t alpha) noexcept {
        value &= (~0xFF) << 24;
        value |= static_cast<std::uint32_t>(alpha) << 24;
    }
    void r(std::uint8_t red) noexcept {
        value &= (~0xFF) << 16;
        value |= static_cast<std::uint32_t>(red) << 16;
    }
    void g(std::uint8_t green) noexcept {
        value &= (~0xFF) << 8;
        value |= static_cast<std::uint32_t>(green) << 8;
    }
    void b(std::uint8_t blue) noexcept {
        value &= (~0xFF);
        value |= static_cast<std::uint32_t>(blue);
    }
};

constexpr color rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
{
    return color(r, g, b);
}
constexpr color argb(std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept
{
    return color(r, g, b, a);
}

namespace colors {

inline constexpr color black = rgb(0, 0, 0);
inline constexpr color white = rgb(255, 255, 255);
inline constexpr color red = rgb(255, 0, 0);
inline constexpr color green = rgb(0, 255, 0);
inline constexpr color blue = rgb(0, 0, 255);
} // namespace colors

}