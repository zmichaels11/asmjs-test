#include "graphics/clear_state_info.hpp"

#include <GLES3/gl3.h>

#include "graphics/clear_buffer.hpp"

namespace graphics {
    void apply(const clear_state_info& info) {
        if ((info.buffers & clear_buffer::COLOR) == clear_buffer::COLOR) {
            glClearColor(info.color.r, info.color.g, info.color.b, info.color.a);
        }

        if ((info.buffers & clear_buffer::DEPTH) == clear_buffer::DEPTH) {
            glClearDepthf(info.depth);
        }

        if ((info.buffers & clear_buffer::STENCIL) == clear_buffer::STENCIL) {
            glClearStencil(info.stencil);
        }

        glClear(static_cast<GLenum> (info.buffers));
    }

    template<>
    clear_state_info defaults() {
        return {
            static_cast<clear_buffer> (0),
            {0.0F, 0.0F, 0.0F, 0.0F},
            1.0F,
            0
        };
    }
}