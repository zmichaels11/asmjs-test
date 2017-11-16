#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class compare_op : GLenum {
        NEVER = GL_NEVER,
        ALWAYS = GL_ALWAYS,
        LESS = GL_LESS,
        GREATER = GL_GREATER,
        LEQUAL = GL_LEQUAL,
        GEQUAL = GL_GEQUAL,
        NOTEQUAL = GL_NOTEQUAL
    };
}