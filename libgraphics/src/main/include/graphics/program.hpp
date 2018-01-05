#pragma once

#include <string>

#include "graphics/program_info.hpp"

namespace graphics {
    class program {
        graphics::program_info _info;
        unsigned int _handle;
        bool _external;

        program(const program&) = delete;

        program& operator= (const program&) = delete;

    public:
        program(program&&) = default;

        program& operator=(program&&) = default;

        program() noexcept: 
            _info(),
            _handle(0),
            _external(false) {}
        
        program(const program_info& info) noexcept;

        program(unsigned int handle) noexcept:
            _info(),
            _handle(handle),
            _external(true) {}        

        ~program() noexcept;        

        unsigned int getUniformBlockIndex(const std::string& name) const noexcept;

        int getUniformLocation(const std::string& name) const noexcept;

        void use() const noexcept;

        const graphics::program_info& getInfo() const noexcept;

        inline operator int() const noexcept {
            return _handle;
        }
    };
}