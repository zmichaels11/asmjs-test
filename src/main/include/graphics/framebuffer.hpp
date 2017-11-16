#pragma once

#include <GLES3/gl3.h>

#include "graphics/framebuffer_info.hpp"

namespace graphics {
    class framebuffer {
        GLuint _handle;
        framebuffer_info _info;

        framebuffer(const framebuffer&) = delete;

        framebuffer& operator=(const framebuffer&) = delete;

    public:
        framebuffer(const framebuffer_info& info);

        framebuffer(framebuffer&&) = default;

        ~framebuffer();

        framebuffer& operator=(framebuffer&&) = default;

        const framebuffer_info& getInfo() const;

        void bind() const;
    };
}