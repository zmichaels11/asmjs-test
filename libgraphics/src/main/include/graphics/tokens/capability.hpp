#pragma once

namespace graphics {
    namespace tokens {
        enum class capability : unsigned int {
            ALPHA_TEST = 0x0001,
            BLEND = 0x0002,
            CULL_FACE = 0x0004,
            DEPTH_TEST = 0x0008,
            SCISSOR_TEST = 0x00010,
            STENCIL_TEST = 0x00020
        };

        constexpr capability operator| (capability lhs, capability rhs) noexcept {
            return static_cast<capability> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
        }

        constexpr capability operator& (capability lhs, capability rhs) noexcept {
            return static_cast<capability> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
        }

        constexpr capability& operator|= (capability& lhs, capability rhs) noexcept {
            return lhs = lhs | rhs;
        }

        constexpr capability& operator&= (capability& lhs, capability rhs) noexcept {
            return lhs = lhs & rhs;
        }
    }
}