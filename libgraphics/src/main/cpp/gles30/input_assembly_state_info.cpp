#if defined(GLES30)

#include "graphics/hstate/input_assembly_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    template<>
    void apply(const input_assembly_state_info& info)  noexcept {        
        if (info.primitiveRestartEnabled) {
            glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
        } else {
            glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
        }
    }
}

#endif