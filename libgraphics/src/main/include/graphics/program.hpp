#pragma once

#include <string>

#include "graphics/program_info.hpp"

namespace graphics {
    class program {
        program_info _info;
        unsigned int _handle;

        program(const program&) = delete;

        program& operator= (const program&) = delete;

    public:
        program() : _handle(0) {}
        
        program(const program_info& info);

        program(program&&) = default;

        ~program();

        program& operator=(program&&) = default;

        unsigned int getUniformBlockIndex(const std::string& name) const;

        int getUniformLocation(const std::string& name) const;

        void use() const;

        const program_info& getInfo() const;
    };
}