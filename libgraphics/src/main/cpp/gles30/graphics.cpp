#if defined(GLES30)

#include "graphics.hpp"

#include <cstdio>

#include <GLES3/gl3.h>

namespace graphics {
    void init() noexcept {
        std::printf("[GLES] OpenGL Version: %s\n", glGetString(GL_VERSION));
        std::printf("[GLES] OpenGL Renderer: %s\n", glGetString(GL_RENDERER));
        std::printf("[GLES] OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
        std::printf("[GLES] OpenGL Shading Language Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
}

#endif