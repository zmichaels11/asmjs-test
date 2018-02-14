#pragma once

#include <string>

#include "graphics/hinfo/program_info.hpp"

namespace graphics {
    class program {
        graphics::program_info _info;
        unsigned int _handle;
        bool _external;
        std::string _name;

        program(const program&) = delete;

        program& operator= (const program&) = delete;

    public:
        inline program(program&& other) noexcept {
            _info = std::move(other._info);
            _name = std::move(other._name);
            _handle = other._handle;
            _external = other._external;            

            other._handle = 0;
        }

        inline program& operator=(program&& other) {
            std::swap(_info, other._info);
            std::swap(_handle, other._handle);
            std::swap(_external, other._external);
            std::swap(_name, other._name);

            return *this;
        }

        program() noexcept: 
            _info(),
            _handle(0),
            _external(false),
            _name("0") {}
        
        program(const program_info& info) noexcept;

        program(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true),
            _name(std::to_string(handle)) {}        

        ~program() noexcept;

        unsigned int getUniformBlockIndex(const std::string& name) const noexcept;

        void setUniformBlockBinding(unsigned int blockIndex, unsigned int bindingPoint) const noexcept;

        int getUniformLocation(const std::string& name) const noexcept;

        void use() const noexcept;

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;

        const graphics::program_info& getInfo() const noexcept;

        inline operator unsigned int() const noexcept {
            return _handle;
        }
    };
}