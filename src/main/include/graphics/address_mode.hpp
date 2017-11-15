#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class address_mode : GLenum {
        REPEAT = GL_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE
    };
}