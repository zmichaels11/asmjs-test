#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class pixel_format : GLenum {
        RGBA = GL_RGBA,
        RGB = GL_RGB,
        RG = GL_RG, 
        RED = GL_RED,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        RGB_INTEGER = GL_RGB_INTEGER,
        RG_INTEGER = GL_RG_INTEGER,
        RED_INTEGER = GL_RED_INTEGER
    };
}