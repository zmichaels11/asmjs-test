#pragma once

namespace engine {
    namespace layers {
        enum class clear_type : unsigned int {
            NONE = 0,
            COLOR = 1,
            DEPTH = 2,
            STENCIL = 4
        };

        constexpr clear_type operator| (clear_type lhs, clear_type rhs) {
            return static_cast<clear_type> (
                static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
        }

        constexpr clear_type operator& (clear_type lhs, clear_type rhs) {
            return static_cast<clear_type> (
                static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
        }
    }
}