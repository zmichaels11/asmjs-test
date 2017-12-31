#include "renderer/render_info.hpp"

namespace renderer {
    template<>
    render_info defaults() {
        return {
            {
                renderer::load_op::LOAD,
                {0.0F, 0.0F, 0.0F, 0.0F}
            },
            {
                renderer::load_op::CLEAR,
                0xFFFFFFFF
            },
            {
                renderer::load_op::LOAD,
                1.0F
            },
            renderer::render_target::COLOR
        };
    }
}