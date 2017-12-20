#ifdef GLES2

#include "graphics/viewport_state_info.hpp"

#include <GLES2/gl2.h>

namespace graphics {
    void apply(const viewport_state_info& info) {
        if (info.minDepth != info.maxDepth) {
            glDepthRangef(info.minDepth, info.maxDepth);
        }

        glViewport(info.x, info.y, info.width, info.height);
    }
}

#endif