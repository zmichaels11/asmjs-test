#pragma once

#include "renderer/info.hpp"
#include "renderer/load_op.hpp"
#include "renderer/render_target.hpp"

namespace renderer {
    struct render_info {
        struct color_t {
            renderer::load_op loadOp;
            struct clear_color_t {
                float r, g, b, a;
            } clearColor;
        } color;

        struct stencil_t {
            renderer::load_op loadOp;
            unsigned int clearStencil;
        } stencil;
        
        struct depth_t {
            renderer::load_op loadOp;
            float clearDepth;
        } depth;
        
        renderer::render_target target;
    };

    template<>
    render_info defaults();
}