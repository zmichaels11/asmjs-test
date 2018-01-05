#pragma once

#include "graphics/framebuffer.hpp"
#include "graphics/renderbuffer_info.hpp"

namespace graphics {    
    class renderbuffer {
        graphics::renderbuffer_info _info;
        unsigned int _handle;
        bool _external;

        renderbuffer(const renderbuffer&) = delete;

        renderbuffer& operator=(const renderbuffer&) = delete;

        friend class framebuffer;

    public:
        renderbuffer(renderbuffer&&) = default;

        renderbuffer& operator=(renderbuffer&&) = default;

        renderbuffer() noexcept: 
            _info(),
            _handle(0),
            _external(false) {}
        
        renderbuffer(const renderbuffer_info& info) noexcept;

        renderbuffer(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true) {}

        ~renderbuffer() noexcept;        

        const graphics::renderbuffer_info& getInfo() const noexcept;
    };
}