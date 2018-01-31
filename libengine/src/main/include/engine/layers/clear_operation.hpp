#pragma once

#include "engine/layers/clear_type.hpp"
#include "engine/layers/color.hpp"

namespace engine {
    namespace layers {
        struct clear_operation {
            clear_type type;
            color clearColor;
            unsigned int clearStencil;
            float clearDepth;

            static constexpr clear_operation noop() noexcept {
                return {clear_type::NONE};
            }

            static constexpr clear_operation colorDepth(const color& color, float depth = 1.0F) noexcept {
                return {
                    clear_type::COLOR | clear_type::DEPTH,
                    color, 0, depth};
            }

            constexpr clear_operation withColor(const color& color) noexcept {
                return {
                    type | clear_type::COLOR,
                    color,
                    clearStencil,
                    clearDepth};
            }

            constexpr clear_operation withDepth(float depth) noexcept {
                return {
                    type | clear_type::DEPTH,
                    clearColor,
                    clearStencil,
                    depth};
            }

            constexpr clear_operation withStencil(unsigned int stencil) noexcept {
                return {
                    type | clear_type::STENCIL,
                    clearColor,
                    clearStencil,
                    clearDepth};
            }
        };
    }
}