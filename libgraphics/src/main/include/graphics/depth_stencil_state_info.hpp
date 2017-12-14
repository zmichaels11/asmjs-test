#pragma once

#include "graphics/compare_op.hpp"
#include "graphics/info.hpp"
#include "graphics/stencil_op_state_info.hpp"

namespace graphics {
    struct depth_stencil_state_info {
        bool depthTestEnabled;
        graphics::compare_op depthCompareOp;
        bool depthWriteEnable;
        bool stencilTestEnable;
        graphics::stencil_op_state_info front;
        graphics::stencil_op_state_info back;
    };

    void apply(const graphics::depth_stencil_state_info& info);

    template<>
    depth_stencil_state_info defaults();
}