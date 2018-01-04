#ifdef GL

#include "graphics/blend_state_info.hpp"

#include "GL/glew.h"

namespace graphics {
    void apply (const blend_state_info& info) {        
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