#pragma once

namespace graphics {
    enum class buffer_access : unsigned int {
        READ = 0x0001,
        WRITE = 0x0002,
        PERSISTENT = 0x0040,
        COHERENT = 0x0080,
        INVALIDATE_RANGE = 0x0004,
        INVALIDATE_BUFFER = 0x0008,
        FLUSH_EXPLICIT = 0x0010,
        UNSYNCHRONIZED = 0x0020
    };

    constexpr graphics::buffer_access operator| (graphics::buffer_access lhs, graphics::buffer_access rhs) {
        return static_cast<buffer_access> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
    }

    constexpr graphics::buffer_access operator& (graphics::buffer_access lhs, graphics::buffer_access rhs) {
        return static_cast<buffer_access> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
    }

    constexpr graphics::buffer_access& operator|= (graphics::buffer_access& lhs, graphics::buffer_access rhs) {
        return lhs = lhs | rhs;
    }

    constexpr graphics::buffer_access& operator&= (graphics::buffer_access& lhs, graphics::buffer_access rhs) {
        return lhs = lhs & rhs;
    }
}