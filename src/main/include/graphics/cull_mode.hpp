#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class cull_mode : GLenum {
        NONE = GL_NONE,
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        FRONT_AND_BACK = GL_FRONT_AND_BACK
    };
}