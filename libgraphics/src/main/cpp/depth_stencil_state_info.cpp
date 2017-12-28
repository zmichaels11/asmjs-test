#include "graphics/depth_stencil_state_info.hpp"

namespace graphics {
    template<>
    depth_stencil_state_info defaults() {
        return {
            false,
            compare_op::LESS,
            true,
            false,
            {stencil_op::KEEP, stencil_op::KEEP, stencil_op::KEEP, compare_op::ALWAYS, 0, 0xFFFF, 0xFFFF},
            {stencil_op::KEEP, stencil_op::KEEP, stencil_op::KEEP, compare_op::ALWAYS, 0, 0xFFFF, 0xFFFF}
        };
    }
}