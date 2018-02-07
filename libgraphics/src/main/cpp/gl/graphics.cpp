#if defined(GL)
#include "pch.h"
#include "graphics.hpp"

namespace {
    void _onError(const std::string& reason, const std::string& msg) noexcept;

    void _onWarning(const std::string& reason, const std::string& msg) noexcept;

    void _onInfo(const std::string& reason, const std::string& msg) noexcept;

    void _onDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam);
}

namespace graphics {                         
    void init() noexcept {
        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            std::cerr << "[GLEW] error: Failed to initialize GLEW!" << std::endl;
            __builtin_trap();
        }

        {
            auto strVersion = std::string(reinterpret_cast<const char * > (glGetString(GL_VERSION)));
            auto strRenderer = std::string(reinterpret_cast<const char * > (glGetString(GL_RENDERER)));
            auto strVendor = std::string(reinterpret_cast<const char * > (glGetString(GL_VENDOR)));
            auto strGLSL = std::string(reinterpret_cast<const char * > (glGetString(GL_SHADING_LANGUAGE_VERSION)));

            std::cout << "[GL] OpenGL Version: " << strVersion << std::endl;
            std::cout << "[GL] OpenGL Renderer: " << strRenderer << std::endl;
            std::cout << "[GL] OpenGL Vendor: " << strVendor << std::endl;
            std::cout << "[GL] OpenGL Shading Language Version: " << strGLSL << std::endl;
        }

        if (GLEW_VERSION_4_3) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(_onDebugMessage, nullptr);            
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
            
            const GLchar * message = "Debug enabled!\0";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1, message);
        } else {
            _onInfo("", "Debug not supported!");
        }
    }
}

namespace {
    void _onError(const std::string& reason, const std::string& msg) noexcept {
        if (reason.empty()) {
            std::cerr << "[GL] error: " << msg << std::endl;
        } else {
            std::cerr << "[GL error (" << reason << "): " << msg << std::endl;
        }

        __builtin_trap();
    }

    void _onWarning(const std::string& reason, const std::string& msg) noexcept {
        if (reason.empty()) {
            std::cerr << "[GL] warning: " << msg << std::endl;
        } else {
            std::cerr << "[GL] warning (" << reason << "): " << msg << std::endl;
        }
    }

    void _onInfo(const std::string& reason, const std::string& msg) noexcept {
        if (reason.empty()) {
            std::cout << "[GL] info: " << msg << std::endl;
        } else {
            std::cout << "[GL] info (" << reason << "): " << msg << std::endl;
        }
    }

    void _onDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
        std::string strMessage;

        if (message == nullptr) {
            strMessage = "<null>";
        } else {
            strMessage = std::string(message, length);
        }

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                _onError("", strMessage);
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                _onWarning("deprecated", strMessage);
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                _onWarning("undefined behavior", strMessage);
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                _onWarning("portability", strMessage);
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                _onWarning("performance", strMessage);
                break;
            case GL_DEBUG_TYPE_MARKER:
                _onInfo("marker", strMessage);
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                _onInfo("push_group", strMessage);
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                _onInfo("pop_group", strMessage);
                break;
            case GL_DEBUG_TYPE_OTHER:
                _onInfo("", strMessage);
                break;
            default:
                _onInfo("unknown", strMessage);
                break;
        }
    }
}
#endif
