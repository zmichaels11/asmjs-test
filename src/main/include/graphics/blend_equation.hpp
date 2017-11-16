#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class blend_equation : GLenum {
        ADD = GL_FUNC_ADD,
        SUBTRACT = GL_FUNC_SUBTRACT,
        REVERSE_SUBTRACT = GL_FUNC_REVERSE_SUBTRACT,
        MIN = GL_MIN,
        MAX = GL_MAX
    };
}