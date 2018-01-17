#pragma once

namespace engine {
    namespace gfx {
        namespace tokens {
            enum class token_type : unsigned int {
                CLEAR = 1,                
                COMPOSITE_MODE = 2,
                DRAW_LINES = 4,
                DRAW_RECTANGLES = 8,
                DRAW_STRING = 16,
                DRAW_TILE = 32,
                DRAW_IMAGE_BASIC = 0x4000,
                DRAW_IMAGE_BC = 0x4100,
                DRAW_IMAGE_HSV = 0x4200,
                DRAW_IMAGE_TINT = 0x4400,
                DRAW_IMAGE_CT = 0x4700
            };

            constexpr token_type operator| (token_type lhs, token_type rhs) {
                return static_cast<token_type>(
                    static_cast<unsigned int> (lhs) | static_cast<unsigned int> (rhs));
            }                                
        }
    }
}