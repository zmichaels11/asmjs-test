#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class index_type : GLenum {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        UNSIGNED_INT = GL_UNSIGNED_INT
    };
}