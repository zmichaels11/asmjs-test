#ifdef GLES30

#include "graphics/compute.hpp"

#include <GLES3/gl31.h>

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