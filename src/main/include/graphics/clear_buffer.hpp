#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class clear_buffer : GLbitfield {
        COLOR = GL_COLOR_BUFFER_BIT,
        DEPTH = GL_DEPTH_BUFFER_BIT,
        STENCIL = GL_STENCIL_BUFFER_BIT
    };

    inline clear_buffer operator| (clear_buffer lhs, clear_buffer rhs) {
        return static_cast<clear_buffer> (static_cast<GLbitfield> (lhs) | static_cast<GLbitfield> (rhs));
    }

    inline clear_buffer operator& (clear_buffer lhs, clear_buffer rhs) {
        return static_cast<clear_buffer> (static_cast<GLbitfield> (lhs) & static_cast<GLbitfield> (rhs));
    }
}