#ifdef GL

#include "graphics/compute.hpp"

#include "GL/glew.h"

namespace graphics {
    namespace compute {
        void dispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) noexcept {
            glDispatchCompute(
                static_cast<GLuint> (numGroupsX), 
                static_cast<GLuint> (numGroupsY), 
                static_cast<GLuint> (numGroupsZ));
        }
    }
}

#endif