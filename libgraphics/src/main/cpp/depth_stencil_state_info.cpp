#include "graphics/hstate/depth_stencil_state_info.hpp"

namespace graphics {    
    depth_stencil_state_info depth_stencil_state_info::defaults() noexcept {
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
