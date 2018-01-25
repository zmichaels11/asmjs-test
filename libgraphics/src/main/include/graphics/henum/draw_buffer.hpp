#pragma once

namespace graphics {
    enum class draw_buffer : unsigned int {
        NONE = 0,
        FRONT_LEFT = 0x0400,
        FRONT_RIGHT = 0x0401,
        BACK_LEFT = 0x0402,
        BACK_RIGHT = 0x0403,
        COLOR_ATTACHMENT0 = 0x8CE0
    };

    constexpr draw_buffer operator+(draw_buffer lhs, int rhs) noexcept {
        return static_cast<draw_buffer> (static_cast<unsigned int> (lhs) + rhs);
    }

    constexpr draw_buffer operator-(draw_buffer lhs, int rhs) noexcept {
        return static_cast<draw_buffer> (static_cast<unsigned int> (lhs) - rhs);
    }

    constexpr draw_buffer& operator+=(draw_buffer& lhs, int rhs) noexcept {
        return lhs = lhs + rhs;
    }

    constexpr draw_buffer& operator-=(draw_buffer& lhs, int rhs) noexcept {
        return lhs = lhs - rhs;
    }
}