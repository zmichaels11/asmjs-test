#ifdef GLES30

#include "graphics/hstate/viewport_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    template<>
    void apply(const viewport_state_info& info) noexcept {
        if (info.minDepth != info.maxDepth) {
            glDepthRangef(info.minDepth, info.maxDepth);
        }

        glViewport(info.x, info.y, info.width, info.height);
    }
}

#endif