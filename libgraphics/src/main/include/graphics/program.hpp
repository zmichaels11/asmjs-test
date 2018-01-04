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
        program() : 
            _info(),
            _handle(0),
            _external(false) {}
        
        program(const program_info& info);

        program(unsigned int handle) :
            _info(),
            _handle(handle),
            _external(true) {}

        program(program&&) = default;

        ~program();

        program& operator=(program&&) = default;

        unsigned int getUniformBlockIndex(const std::string& name) const;

        int getUniformLocation(const std::string& name) const;

        void use() const;

        const graphics::program_info& getInfo() const;

        inline operator int() const {
            return _handle;
        }
    };
}