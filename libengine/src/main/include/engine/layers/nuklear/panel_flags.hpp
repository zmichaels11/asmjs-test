#pragma once

namespace engine {
    namespace layers {
        namespace nuklear {
            enum class panel_flags : unsigned int {
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

            constexpr panel_flags operator| (panel_flags lhs, panel_flags rhs) noexcept {
                return static_cast<panel_flags> (static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
            }

            constexpr panel_flags operator& (panel_flags lhs, panel_flags rhs) noexcept {
                return static_cast<panel_flags> (static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
            }

            constexpr panel_flags& operator|= (panel_flags& lhs, panel_flags rhs) noexcept {
                return lhs = lhs | rhs;
            }

            constexpr panel_flags& operator&= (panel_flags& lhs, panel_flags rhs) noexcept {
                return lhs = lhs & rhs;
            }

            constexpr panel_flags operator^ (panel_flags lhs, panel_flags rhs) noexcept {
                return static_cast<panel_flags> (static_cast<unsigned int> (lhs) ^ static_cast<unsigned int> (rhs));
            }

            constexpr panel_flags& operator^= (panel_flags& lhs, panel_flags rhs) noexcept {
                return lhs = lhs ^ rhs;
            }

            constexpr panel_flags operator~ (panel_flags lhs) noexcept {
                return static_cast<panel_flags> (~static_cast<unsigned int> (lhs));
            }
        }
    }
}