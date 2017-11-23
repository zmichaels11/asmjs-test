#pragma once

namespace graphics {
    enum class clear_buffer : unsigned int {
        COLOR = 0x00004000,
        DEPTH = 0x00000100,
        STENCIL = 0x00000400
    };

    inline clear_buffer operator| (clear_buffer lhs, clear_buffer rhs) {
        return static_cast<clear_buffer> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    inline clear_buffer operator& (clear_buffer lhs, clear_buffer rhs) {
        return static_cast<clear_buffer> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }
}