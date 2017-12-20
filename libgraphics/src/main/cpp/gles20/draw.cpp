#ifdef GLES20

#include <GLES2/gl2.h>

namespace graphics {
    void draw::arrays(draw_mode mode, int first, std::size_t count) {
        glDrawArrays(static_cast<GLenum> (mode), first, count);
    }

    void draw::elements(draw_mode mode, std::size_t count, index_type type, void* offset) {
        glDrawElements(static_cast<GLenum> (mode), count, static_cast<GLenum> (type), offset);
    }

    void draw::arraysInstanced(draw_mode mode, int first, std::size_t count, std::size_t instanceCount) {
        glDrawArraysInstanced(static_cast<GLenum> (mode), first, count, instanceCount);
    }

    void draw::elementsInstanced(draw_mode mode, std::size_t count, index_type type, void * offset, std::size_t instanceCount) {
        glDrawElementsInstanced(static_cast<GLenum> (mode), count, static_cast<GLenum> (type), offset, instanceCount);        
    }

    void draw::rangeElements(draw_mode mode, unsigned int start, unsigned int end, std::size_t count, index_type type, void * offset) {
        glDrawRangeElements(static_cast<GLenum> (mode), start, end, count, static_cast<GLenum> (type), offset);
    }
}

#endif