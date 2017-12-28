#pragma once

#include "graphics/blend_equation.hpp"
#include "graphics/blend_function.hpp"
#include "graphics/info.hpp"

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
    };

    inline blend_state_info blendStatePremultiplyAlpha() {
        return {
            true, 
            graphics::blend_equation::ADD, graphics::blend_equation::ADD,
            graphics::blend_function::ONE, graphics::blend_function::ONE_MINUS_SRC_ALPHA,
            graphics::blend_function::ONE, graphics::blend_function::ONE_MINUS_SRC_ALPHA,
            {true, true, true, true}};
    }

    void apply(const graphics::blend_state_info& info);

    template<>
    blend_state_info defaults();
}