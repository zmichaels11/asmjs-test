#pragma once

#include <GLES3/gl3.h>

namespace graphics {
    enum class buffer_target : GLenum {
        ARRAY = GL_ARRAY_BUFFER,
        ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
        PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER,
        PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
        COPY_READ = GL_COPY_READ_BUFFER,
        COPY_WRITE = GL_COPY_WRITE_BUFFER,
        TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
        UNIFORM_BUFFER = GL_UNIFORM_BUFFER
    };
}