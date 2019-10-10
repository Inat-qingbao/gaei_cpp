#pragma once
#include <cstdint>

namespace gaei {

struct color {
    constexpr color(std::uint8_t red,
                    std::uint8_t green,
                    std::uint8_t blue,
                    std::uint8_t alpha = 0xFF)
        : value(static_cast<std::uint32_t>(alpha) << 24 |
                static_cast<std::uint32_t>(red) << 16 |
                static_cast<std::uint32_t>(green) << 8 |
                static_cast<std::uint32_t>(blue))
        , is_valid{ true }
    {}
    constexpr color()
        : value{}
        , is_valid{ false }
    {}

    std::uint32_t value;
    bool is_valid;

    [[nodiscard]]
    constexpr unsigned int a() const noexcept { return static_cast<unsigned int>(value >> 24); }
    [[nodiscard]]
    constexpr unsigned int r() const noexcept { return static_cast<unsigned int>(value >> 16); }
    [[nodiscard]]
    constexpr unsigned int g() const noexcept { return static_cast<unsigned int>(value >> 8); }
    [[nodiscard]]
    constexpr unsigned int b() const noexcept { return static_cast<unsigned int>(value >> 0); }
    [[nodiscard]]
    constexpr float af() const noexcept { return static_cast<float>(a() / 255.0f); }
    [[nodiscard]]
    constexpr float rf() const noexcept { return static_cast<float>(r() / 255.0f); }
    [[nodiscard]]
    constexpr float gf() const noexcept { return static_cast<float>(g() / 255.0f); }
    [[nodiscard]]
    constexpr float bf() const noexcept { return static_cast<float>(b() / 255.0f); }

    void a(std::uint8_t alpha) noexcept
    {
        is_valid = true;
        value &= ~(0xFFul << 24);
        value |= static_cast<std::uint32_t>(alpha) << 24;
    }
    void r(std::uint8_t red) noexcept
    {
        is_valid = true;
        value &= ~(0xFFul << 16);
        value |= static_cast<std::uint32_t>(red) << 16;
    }
    void g(std::uint8_t green) noexcept
    {
        is_valid = true;
        value &= ~(0xFFul << 8);
        value |= static_cast<std::uint32_t>(green) << 8;
    }
    void b(std::uint8_t blue) noexcept
    {
        is_valid = true;
        value &= ~0xFFul;
        value |= static_cast<std::uint32_t>(blue);
    }

    [[nodiscard]]
    friend constexpr bool operator==(color lhs, color rhs) noexcept
    {
        //       同じ値なら                    ともにfalseなら
        return !(lhs.value ^ rhs.value) || !(lhs.is_valid | rhs.is_valid);
    }
    [[nodiscard]]
    friend constexpr bool operator!=(color lhs, color rhs) noexcept
    {
        return !(lhs == rhs);
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
