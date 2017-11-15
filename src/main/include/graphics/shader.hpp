#pragma once

#include <GLES3/gl3.h>

#include "graphics/shader_info.hpp"

namespace graphics {
    class program;

    class shader {
        shader_info _info;
        GLuint _handle;

        shader(const shader&) = delete;

        shader& operator=(const shader&) = delete;

        friend class program;

    public:
        shader(const shader_info& info);

        shader(shader&&) = default;

        ~shader();

        shader& operator=(shader&&) = default;

        const shader_info& getInfo() const;
    };
}