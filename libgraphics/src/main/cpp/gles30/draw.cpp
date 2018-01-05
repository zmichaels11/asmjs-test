#ifdef GLES30

#include "graphics/draw.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    void draw::arrays(draw_mode mode, int first, std::size_t count) noexcept {
        glDrawArrays(static_cast<GLenum> (mode), first, count);
    }

    void draw::elements(draw_mode mode, std::size_t count, index_type type, void* offset) noexcept {
        glDrawElements(static_cast<GLenum> (mode), count, static_cast<GLenum> (type), offset);
    }

    void draw::arraysInstanced(draw_mode mode, int first, std::size_t count, std::size_t instanceCount) noexcept {
        glDrawArraysInstanced(static_cast<GLenum> (mode), first, count, instanceCount);
    }

    void draw::elementsInstanced(draw_mode mode, std::size_t count, index_type type, void * offset, std::size_t instanceCount) noexcept {
        glDrawElementsInstanced(static_cast<GLenum> (mode), count, static_cast<GLenum> (type), offset, instanceCount);        
    }

    void draw::rangeElements(draw_mode mode, unsigned int start, unsigned int end, std::size_t count, index_type type, void * offset) noexcept {
        glDrawRangeElements(static_cast<GLenum> (mode), start, end, count, static_cast<GLenum> (type), offset);
    }
}

#endif