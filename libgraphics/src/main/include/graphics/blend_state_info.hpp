#pragma once

#include "graphics/blend_equation.hpp"
#include "graphics/blend_function.hpp"

namespace graphics {    
    struct blend_state_info {
        bool blendEnabled;
        graphics::blend_equation colorBlendOp;
        graphics::blend_equation alphaBlendOp;
        graphics::blend_function srcColorBlend;
        graphics::blend_function dstColorBlend;
        graphics::blend_function srcAlphaBlend;
        graphics::blend_function dstAlphaBlend;
        unsigned int colorWriteMask;
    };

    void apply(const graphics::blend_state_info& info);
}