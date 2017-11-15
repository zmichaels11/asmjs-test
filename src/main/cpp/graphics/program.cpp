#include "graphics/program.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <string>

#include "graphics/attribute_state_info.hpp"
#include "graphics/program_info.hpp"
#include "graphics/shader.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << "ERR: " << msg << std::endl;
            __builtin_trap();
        }
    }

    program::program(const program_info& info) {
        _info = info;
        _handle = glCreateProgram();

        for (auto it = info.pShaders; it != (info.pShaders + info.shaderCount); it++) {
            glAttachShader(_handle, it->_handle);
        }

        for (auto it = info.pAttributeStates; it != (info.pAttributeStates + info.attributeCount); it++) {
            glBindAttribLocation(_handle, it->location, it->name.c_str());
        }

        glLinkProgram(_handle);

        auto linkStatus = GL_FALSE;

        glGetProgramiv(_handle, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE) {
            int infoLogLen = 0;

            glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLen);

            char infoLog[infoLogLen];

            glGetProgramInfoLog(_handle, infoLogLen, nullptr, infoLog);

            _onError(infoLog);
        }
    }

    program::~program() {
        glDeleteProgram(_handle);
    }

    void program::use() const {
        glUseProgram(_handle);
    }

    int program::getUniformLocation(const std::string& name) const {
        return glGetUniformLocation(_handle, name.c_str());
    }

    unsigned int program::getUniformBlockIndex(const std::string& name) const {
        return glGetUniformBlockIndex(_handle, name.c_str());
    }

    const program_info& program::getInfo() const {
        return _info;
    }
}