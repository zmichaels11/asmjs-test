#ifdef GLES20

#include "graphics/scissor_state_info.hpp"

#include <GLES2/gl2.h>

namespace graphics {
    void apply(const scissor_state_info& info) {
        if (info.scissorTestEnabled) {
            glEnable(GL_SCISSOR_TEST);
            glScissor(info.left, info.bottom, info.width, info.height);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}

#endif