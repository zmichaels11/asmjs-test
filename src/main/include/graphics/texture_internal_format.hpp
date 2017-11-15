#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class texture_internal_format : GLenum {
        R8 = GL_R8,        
        RG8 = GL_RG8,
        RGB8 = GL_RGB8,
        RGBA8 = GL_RGBA8,
        R16F = GL_R16F,
        RG16F = GL_RG16F,
        RGB16F = GL_RGB16F,
        RGBA16F = GL_RGBA16F,
        RGB10_A2 = GL_RGB10_A2        
    };
}