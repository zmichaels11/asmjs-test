#include "graphics/shader.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <string>

#include "graphics/shader_info.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << "ERR: " << msg << std::endl;
            __builtin_trap();
        }
    }

    shader::shader(const shader_info& info) {
        _info = info;
        _handle = glCreateShader(static_cast<GLenum> (info.type));

        const char * src = info.src.c_str();
        int len = info.src.length();

        glShaderSource(_handle, 1, &src, &len);
        glCompileShader(_handle);

        auto compileStatus = GL_FALSE;

        glGetShaderiv(_handle, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus != GL_TRUE) {
            int infoLogLen = 0;

            glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLen);

            char infoLog[infoLogLen];

            glGetShaderInfoLog(_handle, infoLogLen, nullptr, infoLog);
            _onError(infoLog);
        }
    }

    shader::~shader() {
        glDeleteShader(_handle);
    }

    const shader_info& shader::getInfo() const {
        return _info;
    }
}