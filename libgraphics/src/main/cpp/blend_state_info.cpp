#include "graphics/blend_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    void apply (const blend_state_info& info) {
        if (info.blendEnabled) {
            glEnable(GL_BLEND);
            
            glBlendFuncSeparate(
                static_cast<GLenum> (info.srcColorBlend), static_cast<GLenum> (info.dstColorBlend),
                static_cast<GLenum> (info.srcAlphaBlend), static_cast<GLenum> (info.dstAlphaBlend));
            
            glBlendEquationSeparate(static_cast<GLenum> (info.colorBlendOp), static_cast<GLenum> (info.alphaBlendOp));

            glColorMask(
                (info.colorWriteMask & 0x1), 
                (info.colorWriteMask & 0x2), 
                (info.colorWriteMask & 0x4), 
                (info.colorWriteMask & 0x8));
        } else {
            glDisable(GL_BLEND);
        }
    }

    template<>
    blend_state_info defaults() {
        return {
            false,
            graphics::blend_equation::ADD, graphics::blend_equation::ADD,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            graphics::blend_function::ONE, graphics::blend_function::ZERO,
            0xFFFF};
    }
}