#ifdef GLES30

#include "graphics/scissor_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
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