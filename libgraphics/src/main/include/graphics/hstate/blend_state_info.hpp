#pragma once

#include "graphics/henum/blend_equation.hpp"
#include "graphics/henum/blend_function.hpp"
#include "graphics/hstate/state.hpp"

namespace graphics {    
    struct blend_state_info {
        bool blendEnabled;
        graphics::blend_equation colorBlendOp, alphaBlendOp;
        graphics::blend_function srcColorBlend, dstColorBlend;
        graphics::blend_function srcAlphaBlend, dstAlphaBlend;
        struct color_write_mask_info_t {
            bool red;
            bool green;
            bool blue;
            bool alpha;
        } colorWrite;

        static blend_state_info defaults() noexcept;

        static blend_state_info premultipliedAlpha() noexcept;
    };    

    template<>
    void apply(const blend_state_info& info) noexcept;
}