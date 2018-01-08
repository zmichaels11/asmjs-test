#ifdef GL

#include "graphics/hstate/clear_state_info.hpp"

#include "GL/glew.h"

#include "graphics/hbitfield/clear_buffer.hpp"

namespace graphics {
    template<>
    void apply(const clear_state_info& info) noexcept {       
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
}

#endif