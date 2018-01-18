#pragma once

namespace engine {
    namespace layers {
        enum class sprite_sheet_hint : unsigned int {
            LAYERED = 1,
            HORIZONTAL_REPEAT = 2,
            VERTICAL_REPEAT = 4,
            GENERATE_MIPMAP = 8,
            GLOBAL = 16
        };

        constexpr sprite_sheet_hint operator| (sprite_sheet_hint lhs, sprite_sheet_hint rhs) {
            return static_cast<sprite_sheet_hint> (
                static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
        }

        constexpr sprite_sheet_hint operator& (sprite_sheet_hint lhs, sprite_sheet_hint rhs) {
            return static_cast<sprite_sheet_hint> (
                static_cast<unsigned int> (lhs) & static_cast<unsigned int> (rhs));
        }
    }
}