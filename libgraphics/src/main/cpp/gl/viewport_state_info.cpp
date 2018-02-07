#if defined(GL)
#include "pch.h"
#include "graphics/hstate/viewport_state_info.hpp"

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
