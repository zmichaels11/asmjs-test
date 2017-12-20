#ifdef GL45

#include "graphics/compute.hpp"

#include <GL/glew.h>

namespace graphics {
    namespace compute {
        void dispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) {
            glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
        }
    }
}

#endif