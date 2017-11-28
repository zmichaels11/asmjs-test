#include "graphics/viewport_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    void apply(const viewport_state_info& info) {
        if (info.minDepth != info.maxDepth) {
            glDepthRangef(info.minDepth, info.maxDepth);
        }

        glViewport(info.x, info.y, info.width, info.height);
    }
}