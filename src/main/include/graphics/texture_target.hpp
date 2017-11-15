#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class texture_target : GLenum {
        TEXTURE_2D = GL_TEXTURE_2D,
        TEXTURE_3D = GL_TEXTURE_3D,
        TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY        
    }
}