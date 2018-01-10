#pragma once

#include <string>

#include "graphics/hinfo/shader_info.hpp"

namespace graphics {
    class program;

    class shader {
        graphics::shader_info _info;
        unsigned int _handle;
        bool _external;
        std::string _name;

        shader(const shader&) = delete;

        shader& operator=(const shader&) = delete;

        friend class program;

    public:
        shader(shader&&) = default;

        shader& operator=(shader&&) = default;

        shader() noexcept: 
            _info(),
            _handle(0),
            _external(false),
            _name("0") {}
        
        shader(const shader_info& info) noexcept;

        shader(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true),
            _name(std::to_string(handle)) {}        

        ~shader() noexcept;

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;

        const graphics::shader_info& getInfo() const noexcept;
    };
}