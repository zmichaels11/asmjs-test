#if defined(GLES30)
#include "pch.h"
#include "graphics/hobject/shader.hpp"

#include "graphics/hinfo/shader_info.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    shader::shader(const shader_info& info) noexcept {
        _info = info;
        _handle = glCreateShader(static_cast<GLenum> (info.type));
        _external = false;

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

        _name = std::to_string(_handle);
    }

    shader::~shader() noexcept {
        if (!_handle && _external) {
            glDeleteShader(_handle);
            _handle = 0;
        }
    }

    void shader::setName(const std::string& name) noexcept {
        _name = name;
    }

    const std::string& shader::getName() const noexcept {
        return _name;
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[GLES] shader error: " << msg << std::endl;
            __builtin_trap();
        }
    }
}
#endif
