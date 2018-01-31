#if defined(GL)

#include "graphics/hutil/compute.hpp"

#include <GL/glew.h>

namespace graphics {
    void compute::dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) noexcept {
        glDispatchCompute(
            static_cast<GLuint> (numGroupsX), 
            static_cast<GLuint> (numGroupsY), 
            static_cast<GLuint> (numGroupsZ));        
    }
}

#endif