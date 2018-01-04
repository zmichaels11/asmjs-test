#ifdef GL
#include "graphics.hpp"

#include "GL/glew.h"

#include <iostream>

namespace graphics {
    void init() {
        glewExperimental = true;

        if (glewInit() != GLEW_OK) {
            std::cerr << "ERR: Unable to initialize GLEW!" << std::endl;
            __builtin_trap();
        }

        std::cout << "OpenGL Version: "
            << glGetString(GL_VERSION)
            << "\nOpenGL Renderer: "
            << glGetString(GL_RENDERER)
            << "\nOpenGL Vendor: "
            << glGetString(GL_VENDOR)
            << "\nGLSL Version: "
            << glGetString(GL_SHADING_LANGUAGE_VERSION)
            << "\nExtensions: "
            << glGetString(GL_EXTENSIONS)
            << std::endl;
    }
}
#endif