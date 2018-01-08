#pragma once

#include "graphics/hbitfield/clear_buffer.hpp"
#include "graphics/hstate/state.hpp"

namespace graphics {
    struct clear_state_info {
        graphics::clear_buffer buffers;
        struct clear_color_t {
            float r, g, b, a;
        } color;
        float depth;
        unsigned int stencil;

        static clear_state_info defaults() noexcept;
    };

    template<>
    void apply(const graphics::clear_state_info& info) noexcept;
}