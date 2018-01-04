#pragma once

#include "graphics/program.hpp"
#include "graphics/shader_info.hpp"

namespace graphics {
    class shader {
        graphics::shader_info _info;
        unsigned int _handle;
        bool _external;

        shader(const shader&) = delete;

        shader& operator=(const shader&) = delete;

        friend class program;

    public:
        shader() : 
            _info(),
            _handle(0),
            _external(false) {}
        
        shader(const shader_info& info);

        shader(unsigned int handle) :
            _info(),
            _handle(handle),
            _external(true) {}

        shader(shader&&) = default;

        ~shader();

        shader& operator=(shader&&) = default;

        const graphics::shader_info& getInfo() const;
    };
}