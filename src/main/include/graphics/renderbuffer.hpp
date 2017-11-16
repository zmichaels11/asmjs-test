#pragma once

#include <GLES3/gl3.h>

#include "graphics/renderbuffer_info.hpp"

namespace graphics {
    class framebuffer;
    
    class renderbuffer {
        renderbuffer_info _info;
        GLuint _handle;

        renderbuffer(const renderbuffer&) = delete;

        renderbuffer& operator=(const renderbuffer&) = delete;

        friend class framebuffer;

    public:
        renderbuffer(const renderbuffer_info& info);

        ~renderbuffer();

        renderbuffer(renderbuffer&&) = default;

        renderbuffer& operator=(renderbuffer&&) = default;

        const renderbuffer_info& getInfo() const;
    };
}