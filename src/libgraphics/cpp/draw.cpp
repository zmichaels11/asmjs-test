#include "graphics/draw.hpp"

#include <GLES3/gl3.h>

namespace graphics {    
    void draw::drawArrays(draw_mode mode, int first, std::size_t count) {
        glDrawArrays(static_cast<GLenum> (mode), first, count);
    }

    void draw::drawElements(draw_mode mode, std::size_t count, index_type type, void* offset) {
        glDrawElements(static_cast<GLenum> (mode), count, static_cast<GLenum> (type), offset);
    }

    void draw::drawArraysInstanced(draw_mode mode, int first, std::size_t count, std::size_t instanceCount) {
        glDrawArraysInstanced(static_cast<GLenum> (mode), first, count, instanceCount);
    }

    void draw::drawElementsInstanced(draw_mode mode, std::size_t count, index_type type, void * offset, std::size_t instanceCount) {
        glDrawElementsInstanced(static_cast<GLenum> (mode), count, static_cast<GLenum> (type), offset, instanceCount);        
    }

    void draw::drawRangeElements(draw_mode mode, unsigned int start, unsigned int end, std::size_t count, index_type type, void * offset) {
        glDrawRangeElements(static_cast<GLenum> (mode), start, end, count, static_cast<GLenum> (type), offset);
    }
}