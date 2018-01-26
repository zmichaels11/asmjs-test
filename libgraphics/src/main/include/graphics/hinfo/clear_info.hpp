#pragma once

namespace graphics {
    struct clear_info {
        union value_t {
            struct color_t {
                float r, g, b, a;
            } color;
            float depth;
            unsigned int stencil;
        } value;
    };
}