#pragma once

#include "graphics/compare_op.hpp"
#include "graphics/stencil_op_state_info.hpp"

namespace graphics {
    struct depth_stencil_state_info {
        bool depthTestEnabled;
        compare_op depthCompareOp;
        bool depthWriteEnable;
        bool stencilTestEnable;
        stencil_op_state_info front;
        stencil_op_state_info back;
    };

    void apply(const depth_stencil_state_info& info);
}