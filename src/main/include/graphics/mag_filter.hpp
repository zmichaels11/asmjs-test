#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class mag_filter : GLenum {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR
    };
}