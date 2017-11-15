#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class shader_type : GLenum {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER
    };
}