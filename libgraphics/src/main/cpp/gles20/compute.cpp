#ifdef GLES20

#include <iostream>

namespace graphics {
    namespace compute {
        void dispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ) {
            std::cerr << "Compute is not supported on OpenGL ES 2.0!";
            __builtin_trap();
        } 
    }
}
#endif