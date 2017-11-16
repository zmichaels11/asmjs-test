#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class draw_mode : GLenum {
        POINTS = GL_POINTS,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        LINES = GL_LINES,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
        TRIANGLES = GL_TRIANGLES
    };
}

