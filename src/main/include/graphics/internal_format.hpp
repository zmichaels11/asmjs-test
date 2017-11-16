#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class internal_format : GLenum {
        R8 = GL_R8,        
        RG8 = GL_RG8,
        RGB8 = GL_RGB8,
        RGBA8 = GL_RGBA8,
        R16F = GL_R16F,
        RG16F = GL_RG16F,
        RGB16F = GL_RGB16F,
        RGBA16F = GL_RGBA16F,
        RGB10_A2 = GL_RGB10_A2,
        RGBA4 = GL_RGBA4,
        RGB565 = GL_RGB565,
        RGB5_A1 = GL_RGB5_A1,                
        DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
        DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
        STENCIL_INDEX8 = GL_STENCIL_INDEX8
    };
}