#ifdef GLES30

#include "graphics/shader.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/shader_info.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg);
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

    namespace {

        void _onError(const std::string& msg) {
            std::cerr << "shader error: " << msg << std::endl;
            std::cerr << "Supported GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
            std::cerr << "Supported version: " << glGetString(GL_VERSION) << std::endl;

            __builtin_trap();
        }
    }
}

#endif