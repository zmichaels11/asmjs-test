#include "graphics/compute.hpp"

#include <GLES3/gl31.h>

namespace graphics {
    namespace compute {
        void dispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) {
            glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
        }
    }
}