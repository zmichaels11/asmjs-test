#pragma once

#include "graphics/program.hpp"
#include "graphics/shader_info.hpp"

namespace graphics {
    class shader {
        graphics::shader_info _info;
        unsigned int _handle;

        shader(const shader&) = delete;

        shader& operator=(const shader&) = delete;

        friend class program;

    public:
        shader() : _handle(0) {}
        
        shader(const shader_info& info);

        shader(shader&&) = default;

        ~shader();

        shader& operator=(shader&&) = default;

        const graphics::shader_info& getInfo() const;
    };
}