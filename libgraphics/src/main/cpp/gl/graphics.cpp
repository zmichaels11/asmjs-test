#if defined(GL)
#include "graphics.hpp"

#include <cstdio>

#include <GL/glew.h>

#include <string>

void onDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
    if (message == nullptr) {
        std::printf("Message is null!\n");
        return;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::printf("[GL] Error: %s\n", message);
            __builtin_trap();
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::printf("[GL] Warning (deprecated): %s\n", message);
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::printf("[GL] Warning (undefined behavior): %s\n", message);
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::printf("[GL] Warning (portability): %s\n", message);
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::printf("[GL] Warning (performance): %s\n", message);
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::printf("[GL] Info (marker): %s\n", message);
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::printf("[GL] Info (push_group): %s\n", message);
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::printf("[GL] Info (pop_group): %s\n", message);
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::printf("[GL] Info: %s\n", message);
            break;
        default:
            std::printf("[GL] Unknown debug message!\n");
            break;
    }
}

namespace graphics {                         
    void init() noexcept {
        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            std::printf("[GLEW] Error: Failed to initialize GLEW!\n");
            __builtin_trap();
        }

        std::printf("[GL] OpenGL Version: %s\n", glGetString(GL_VERSION));
        std::printf("[GL] OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
        std::printf("[GL] OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
        std::printf("[GL] OpenGL Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

        if (GLEW_VERSION_4_3) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(onDebugMessage, nullptr);            
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
            
            const GLchar * message = "Debug enabled!\0";
            glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, sizeof(message), message);
        } else {
            std::printf("[GL] Debug not supported!\n");
        }
    }
}
#endif