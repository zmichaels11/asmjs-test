#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class front_face : GLenum {
        CCW = GL_CCW,
        CW = GL_CW
    };
}