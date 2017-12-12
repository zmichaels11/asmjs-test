#pragma once

#include "graphics/framebuffer.hpp"
#include "graphics/renderbuffer_info.hpp"

namespace graphics {    
    class renderbuffer {
        graphics::renderbuffer_info _info;
        unsigned int _handle;

        renderbuffer(const renderbuffer&) = delete;

        renderbuffer& operator=(const renderbuffer&) = delete;

        friend class framebuffer;

    public:
        renderbuffer() : _handle(0) {}
        
        renderbuffer(const renderbuffer_info& info);

        ~renderbuffer();

        renderbuffer(renderbuffer&&) = default;

        renderbuffer& operator=(renderbuffer&&) = default;

        const graphics::renderbuffer_info& getInfo() const;
    };
}