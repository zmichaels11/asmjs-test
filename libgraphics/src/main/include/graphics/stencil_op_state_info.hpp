#pragma once

#include "graphics/compare_op.hpp"
#include "graphics/stencil_op.hpp"

namespace graphics {
    struct stencil_op_state_info {
        stencil_op failOp;
        stencil_op passOp;
        stencil_op depthFailOp;
        compare_op compareOp;
        int reference;
        unsigned int compareMask;
        unsigned int writeMask;
    };
}