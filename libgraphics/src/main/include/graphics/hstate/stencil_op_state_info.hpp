#pragma once

#include "graphics/henum/compare_op.hpp"
#include "graphics/henum/stencil_op.hpp"

namespace graphics {
    struct stencil_op_state_info {
        graphics::stencil_op failOp;
        graphics::stencil_op passOp;
        graphics::stencil_op depthFailOp;
        graphics::compare_op compareOp;
        int reference;
        unsigned int compareMask;
        unsigned int writeMask;

        static stencil_op_state_info defaults() noexcept;
    };
}