#pragma once

#include <cstdint>

namespace engine {
    namespace gui {
        struct color4ub {
            std::uint8_t r, g, b, a;
        };

        constexpr bool operator==(color4ub lhs, color4ub rhs) {
            return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
        }

        constexpr bool operator!=(color4ub lhs, color4ub rhs) {
            return lhs.r != rhs.r || lhs.g != rhs.g || lhs.b != rhs.b || lhs.a != rhs.a;
        }
    }
}