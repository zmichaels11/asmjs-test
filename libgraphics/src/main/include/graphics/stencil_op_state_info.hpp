#pragma once

#include "graphics/compare_op.hpp"
#include "graphics/stencil_op.hpp"

namespace graphics {
    struct stencil_op_state_info {
        graphics::stencil_op failOp;
        graphics::stencil_op passOp;
        graphics::stencil_op depthFailOp;
        graphics::compare_op compareOp;
        int reference;
        unsigned int compareMask;
        unsigned int writeMask;
    };
}