#ifdef GL

#include "graphics/shader.hpp"

#include "GL/glew.h"

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
        _external = false;

        auto src = static_cast<const GLchar*> (info.src.c_str());
        auto len = static_cast<GLint> (info.src.length());

        _handle = glCreateShader(static_cast<GLenum> (info.type));        

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
        if (_handle && !_external) {
            glDeleteShader(_handle);
            _handle = 0;
        }
    }

    namespace {

        void _onError(const std::string& msg) {
            std::cerr << "shader error: " << msg << std::endl;
            __builtin_trap();
        }
    }
}

#endif