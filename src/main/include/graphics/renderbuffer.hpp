#pragma once

#include "graphics/renderbuffer_info.hpp"

namespace graphics {
    class framebuffer;
    
    class renderbuffer {
        renderbuffer_info _info;
        unsigned int _handle;

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