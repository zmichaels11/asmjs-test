#ifdef GL

#include "graphics/program.hpp"

#include "GL/glew.h"

#include <iostream>
#include <string>

#include "graphics/attribute_state_info.hpp"
#include "graphics/program_info.hpp"
#include "graphics/shader.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg);
    }

    program::program(const program_info& info) {
        _info = info;
        _handle = glCreateProgram();
        _external = false;

        for (int i = 0; i < info.shaderCount; i++) {
            glAttachShader(_handle, info.ppShaders[i]->_handle);
        }

        for (auto it = info.pAttributeStates; it != (info.pAttributeStates + info.attributeCount); it++) {
            glBindAttribLocation(_handle, it->location, it->name.c_str());
        }

        glLinkProgram(_handle);

        for (int i = 0; i < info.shaderCount; i++) {
            glDetachShader(_handle, info.ppShaders[i]->_handle);
        }

        auto linkStatus = GL_FALSE;

        glGetProgramiv(_handle, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE) {
            int infoLogLen;

            glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLen);

            char infoLog[infoLogLen];

            glGetProgramInfoLog(_handle, infoLogLen, nullptr, infoLog);

            _onError(infoLog);
        }
    }

    program::~program() {
        if (_handle && !_external) {
            glDeleteProgram(_handle);
            _handle = 0;
        }
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

    namespace {
        void _onError(const std::string& msg)  {
            std::cerr << "program error: " << msg << std::endl;
            __builtin_trap();
        }
    }
}

#endif