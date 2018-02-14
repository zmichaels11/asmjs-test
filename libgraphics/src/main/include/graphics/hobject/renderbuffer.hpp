#pragma once

#include <string>
#include <utility>

#include "graphics/hinfo/renderbuffer_info.hpp"

namespace graphics {    
    class framebuffer;
    
    class renderbuffer {
        graphics::renderbuffer_info _info;
        unsigned int _handle;
        bool _external;
        std::string _name;

        renderbuffer(const renderbuffer&) = delete;

        renderbuffer& operator=(const renderbuffer&) = delete;

        friend class framebuffer;

    public:
        inline renderbuffer(renderbuffer&& other) noexcept {
            _info = std::move(other._info);
            _name = std::move(other._name);
            _handle = other._handle;
            _external = other._external;

            other._handle = 0;
        }

        inline renderbuffer& operator=(renderbuffer&& other) {
            std::swap(_info, other._info);
            std::swap(_name, other._name);
            std::swap(_handle, other._handle);
            std::swap(_external, other._external);

            return *this;
        }

        renderbuffer() noexcept: 
            _info(),
            _handle(0),
            _external(false),
            _name("0") {}
        
        renderbuffer(const renderbuffer_info& info) noexcept;

        renderbuffer(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true),
            _name(std::to_string(handle)) {}

        ~renderbuffer() noexcept;

        inline operator unsigned int() const noexcept {
            return _handle;
        }

        void setName(const std::string& name) noexcept;   

        const std::string& getName() const noexcept;

        const graphics::renderbuffer_info& getInfo() const noexcept;
    };
}