#if defined(GL)
#include "pch.h"
#include "graphics/hstate/blend_state_info.hpp"

namespace graphics {
    template<>
    void apply (const blend_state_info& info) noexcept {        
        if (info.blendEnabled) {
            glEnable(GL_BLEND);
            
            glBlendFuncSeparate(
                static_cast<GLenum> (info.srcColorBlend), static_cast<GLenum> (info.dstColorBlend),
                static_cast<GLenum> (info.srcAlphaBlend), static_cast<GLenum> (info.dstAlphaBlend));
            
            glBlendEquationSeparate(static_cast<GLenum> (info.colorBlendOp), static_cast<GLenum> (info.alphaBlendOp));

            glColorMask(
                static_cast<GLboolean> (info.colorWrite.red), 
                static_cast<GLboolean> (info.colorWrite.green), 
                static_cast<GLboolean> (info.colorWrite.blue), 
                static_cast<GLboolean> (info.colorWrite.alpha));
        } else {
            glDisable(GL_BLEND);
        }
    }
}
#endif
