#pragma once

#include "graphics/henum/compare_op.hpp"
#include "graphics/hstate/state.hpp"
#include "graphics/hstate/stencil_op_state_info.hpp"

namespace graphics {
    struct depth_stencil_state_info {
        bool depthTestEnabled;
        graphics::compare_op depthCompareOp;
        bool depthWriteEnable;
        bool stencilTestEnable;
        graphics::stencil_op_state_info front;
        graphics::stencil_op_state_info back;

        static depth_stencil_state_info defaults() noexcept;
    };

    template<>
    void apply(const depth_stencil_state_info& info) noexcept;
}