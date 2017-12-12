#pragma once

namespace graphics {
    enum class clear_buffer : unsigned int {
        COLOR = 0x00004000,
        DEPTH = 0x00000100,
        STENCIL = 0x00000400
    };

    constexpr graphics::clear_buffer operator| (graphics::clear_buffer lhs, graphics::clear_buffer rhs) {
        return static_cast<clear_buffer> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr graphics::clear_buffer operator& (graphics::clear_buffer lhs, graphics::clear_buffer rhs) {
        return static_cast<clear_buffer> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}