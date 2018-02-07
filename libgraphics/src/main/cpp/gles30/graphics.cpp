#if defined(GLES30)
#include "pch.h"
#include "graphics.hpp"

namespace graphics {
    void init() noexcept {
        auto strVersion = std::string(reinterpret_cast<const char * > (glGetString(GL_VERSION)));
        auto strRenderer = std::string(reinterpret_cast<const char * > (glGetString(GL_RENDERER)));
        auto strVendor = std::string(reinterpret_cast<const char * > (glGetString(GL_VENDOR)));
        auto strGLSL = std::string(reinterpret_cast<const char * > (glGetString(GL_SHADING_LANGUAGE_VERSION)));

        std::cout << "[GLES] OpenGLES Version: " << strVersion << std::endl;
        std::cout << "[GLES] OpenGLES Renderer: " << strRenderer << std::endl;
        std::cout << "[GLES] OpenGLES Vendor: " << strVendor << std::endl;
        std::cout << "[GLES] OpenGLES Shading Language Version: " << strGLSL << std::endl;
    }
}
#endif
