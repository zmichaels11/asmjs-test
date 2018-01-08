#pragma once

#include "graphics/hinfo/shader_info.hpp"

namespace graphics {
    class program;

    class shader {
        graphics::shader_info _info;
        unsigned int _handle;
        bool _external;

        shader(const shader&) = delete;

        shader& operator=(const shader&) = delete;

        friend class program;

    public:
        shader(shader&&) = default;

        shader& operator=(shader&&) = default;

        shader() noexcept: 
            _info(),
            _handle(0),
            _external(false) {}
        
        shader(const shader_info& info) noexcept;

        shader(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true) {}        

        ~shader() noexcept;        

        const graphics::shader_info& getInfo() const noexcept;
    };
}