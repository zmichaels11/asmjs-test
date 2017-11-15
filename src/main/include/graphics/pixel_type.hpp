#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class pixel_type :GLenum {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        BYTE = GL_BYTE,
        SHORT = GL_SHORT,
        INT = GL_INT,
        HALF_FLOAT = GL_HALF_FLOAT,
        FLOAT = GL_FLOAT,
        UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
        UNSIGNED_INT_5_9_9_9_REV = GL_UNSIGNED_INT_5_9_9_9_REV,
        UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
        UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
        UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4        
    };
}