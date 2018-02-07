#if defined(GL)
#include "pch.h"
#include "graphics/hstate/scissor_state_info.hpp"

namespace graphics {
    template<>
    void apply(const scissor_state_info& info) noexcept {
        if (info.scissorTestEnabled) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(info.left, info.bottom, info.width, info.height);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}
#endif
