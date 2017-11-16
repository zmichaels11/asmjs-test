#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class stencil_op : GLenum {
        KEEP = GL_KEEP,
        ZERO = GL_ZERO,
        REPLACE = GL_REPLACE,
        INCR = GL_INCR,
        DECR = GL_DECR,
        INVERT = GL_INVERT,
        INCR_WRAP = GL_INCR_WRAP,
        DECR_WRAP = GL_DECR_WRAP
    };
}