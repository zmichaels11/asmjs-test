#if defined(GL)

#include "graphics/hobject/shader.hpp"

#include <cstdio>

#include <GL/glew.h>

#include <memory>
#include <string>

#include "graphics/hinfo/shader_info.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    shader::shader(const shader_info& info) noexcept {        
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

        _name = std::to_string(_handle);
    }

    shader::~shader() noexcept {
        if (_handle && !_external) {
            glDeleteShader(_handle);
            _handle = 0;
        }
    }

    void shader::setName(const std::string& name) noexcept {
        _name = name;

        if (GLEW_VERSION_4_3) {
            glObjectLabel(GL_SHADER, _handle, _name.size(), _name.c_str());
        }
    }

    const std::string& shader::getName() const noexcept {
        return _name;
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::printf("[GL] Shader error: %s\n", msg.c_str());
            __builtin_trap();
        }
    }
}

#endif