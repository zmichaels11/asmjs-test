#pragma once

#include "graphics/blend_equation.hpp"
#include "graphics/blend_function.hpp"

namespace graphics {
    struct blend_state_info {
        bool blendEnabled;
        blend_equation colorBlendOp;
        blend_equation alphaBlendOp;
        blend_function srcColorBlend;
        blend_function dstColorBlend;
        blend_function srcAlphaBlend;
        blend_function dstAlphaBlend;
        unsigned int colorWriteMask;
    };

    void apply(const blend_state_info& info);
}