#include "graphics/shader.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/shader_info.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << "shader error: " << msg << std::endl;
            __builtin_trap();
        }
    }

    shader::shader(const shader_info& info) {
        _info = info;
        _handle = glCreateShader(static_cast<GLenum> (info.type));

        auto src = static_cast<const GLchar*> (info.src.c_str());
        auto len = static_cast<GLint> (info.src.length());

        glShaderSource(_handle, 1, &src, &len);
        glCompileShader(_handle);

        auto compileStatus = GL_FALSE;

        glGetShaderiv(_handle, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus != GL_TRUE) {
            int infoLogLen;

            glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &infoLogLen);

            auto infoLog = std::make_unique<char[]> (infoLogLen);

            glGetShaderInfoLog(_handle, infoLogLen, nullptr, infoLog.get());

            auto log = std::string(infoLog.get()) + "\n" + src;
            
            _onError(log);
        }
    }

    shader::~shader() {
        glDeleteShader(_handle);
    }

    const shader_info& shader::getInfo() const {
        return _info;
    }
}