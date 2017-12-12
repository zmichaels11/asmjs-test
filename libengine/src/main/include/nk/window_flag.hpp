#pragma once

namespace nk {
    enum class window_flag {        
        BORDER = 1 << 0,
        MOVABLE = 1 << 1,
        SCALABLE = 1 << 2,
        CLOSABLE = 1 << 3,
        MINIMIZABLE = 1 << 4,
        NO_SCROLLBAR = 1 << 5,
        TITLE = 1 << 6,
        SCROLL_AUTO_HIDE = 1 << 7,
        BACKGROUND = 1 << 8,
        SCALE_LEFT = 1 << 9,
        NO_INPUT = 1 << 10
    };

    constexpr window_flag operator| (window_flag lhs, window_flag rhs) {
        return static_cast<window_flag> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr window_flag operator& (window_flag lhs, window_flag rhs) {
        return static_cast<window_flag> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }

    constexpr window_flag& operator|= (window_flag& lhs, window_flag rhs) {
        return (lhs = lhs | rhs);
    }
}