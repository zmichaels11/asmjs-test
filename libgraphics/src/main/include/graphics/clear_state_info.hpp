#pragma once

#include "graphics/clear_buffer.hpp"

namespace graphics {
    struct clear_state_info {
        graphics::clear_buffer buffers;
        struct clear_color_t {
            float r, g, b, a;
        } color;
        float depth;
        unsigned int stencil;
    };

    void apply(const graphics::clear_state_info& info);
}