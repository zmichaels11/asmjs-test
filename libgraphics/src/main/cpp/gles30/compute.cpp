#if defined(GLES30)
#include "pch.h"
#include "graphics/hutil/compute.hpp"

namespace graphics {
    
    void compute::dispatch(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) noexcept {
        glDispatchCompute(
            static_cast<GLuint> (numGroupsX), 
            static_cast<GLuint> (numGroupsY), 
            static_cast<GLuint> (numGroupsZ));
    }    
}
#endif
