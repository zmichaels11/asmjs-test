#ifdef GL45

#include "graphics/buffer.hpp"

#include <GL/glew.h>

#include <iostream>

#include "graphics/buffer_info.hpp"
#include "graphics/buffer_target.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }
    }

    buffer::buffer(const buffer_info& info) {
        _info = info;
        
        glCreateBuffers(1, &_handle);

        auto tgt = static_cast<GLenum> (info.target);
        auto usage = static_cast<GLenum> (info.usage);

        glNamedBufferData(_handle, info.initialData.size, info.initialData.pData, usage);        
    }

    buffer::~buffer() {
        if (_handle) {
            glDeleteBuffers(1, &_handle);            
        }
    }

    void buffer::invalidate() const {
        glInvalidateBufferData(_handle);
    }

    void buffer::bind(buffer_target target) const {
        glBindBuffer(static_cast<GLenum> (target), _handle);        
    }

    void buffer::subData(long offset, const void * src, std::size_t size) const {
        auto tgt = static_cast<GLenum> (_info.target);

        glNamedBufferSubData(_handle, offset, size, src);
    }

    void buffer::bindBase(unsigned int index) const {
        glBindBufferBase(static_cast<GLenum> (_info.target), index, _handle);
    }

    void buffer::bindRange(unsigned int index, long offset, std::size_t size) const {
        glBindBufferRange(static_cast<GLenum> (_info.target), index, _handle, offset, size);
    }
}

#endif