#if defined(GL)

#include "graphics/hobject/program.hpp"

#include <GL/glew.h>

#include <iostream>
#include <string>

#include "graphics/hinfo/attribute_state_info.hpp"
#include "graphics/hinfo/program_info.hpp"
#include "graphics/hobject/shader.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    program::program(const program_info& info) noexcept {
        _info = info;
        _handle = glCreateProgram();
        _external = false;

        for (decltype(info.nShaders) i = 0; i < info.nShaders; i++) {
            glAttachShader(_handle, info.ppShaders[i]->_handle);
        }

        for (decltype(info.nAttributes) i = 0; i < info.nAttributes; i++) {
            auto& attrib = info.pAttributes[i];

            glBindAttribLocation(_handle, attrib.location, attrib.name.c_str());
        }

        glLinkProgram(_handle);

        for (decltype(info.nShaders) i = 0; i < info.nShaders; i++) {
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

        _name = std::to_string(_handle);
    }

    program::~program() noexcept {
        if (_handle && !_external) {
            glDeleteProgram(_handle);
            _handle = 0;
        }
    }

    void program::setName(const std::string& name) noexcept {
        _name = name;

        if (GLEW_VERSION_4_3) {
            glObjectLabel(GL_PROGRAM, _handle, _name.size(), _name.c_str());
        }
    }

    const std::string& program::getName() const noexcept {
        return _name;
    }

    void program::use() const noexcept {
        glUseProgram(_handle);
    }

    int program::getUniformLocation(const std::string& name) const noexcept {
        return glGetUniformLocation(_handle, name.c_str());
    }

    unsigned int program::getUniformBlockIndex(const std::string& name) const noexcept {
        return glGetUniformBlockIndex(_handle, name.c_str());
    }

    void program::setUniformBlockBinding(unsigned int blockIndex, unsigned int bindingPoint) const noexcept {
        glUniformBlockBinding(_handle, blockIndex, bindingPoint);
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[GL] program error: " << msg << std::endl;
            __builtin_trap();
        }
    }
}

#endif