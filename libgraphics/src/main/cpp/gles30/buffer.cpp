#ifdef GLES30

#include "graphics/buffer.hpp"

#include <GLES3/gl3.h>

#include "graphics/buffer_info.hpp"
#include "graphics/buffer_target.hpp"

namespace graphics {
    buffer::buffer(const buffer_info& info) noexcept {
        _info = info;
        _handle = 0;
        _external = false;      

        auto tgt = static_cast<GLenum> (info.target);
        auto usage = static_cast<GLenum> (info.usage);

        glGenBuffers(1, &_handle);
        glBindBuffer(tgt, _handle);
        glBufferData(tgt, info.initialData.size, info.initialData.pData, usage);
    }

    buffer::~buffer() noexcept {
        if (_handle && !_external) {
            glDeleteBuffers(1, &_handle);
            _handle = 0;
        }
    }

    void buffer::invalidate() const noexcept {
        auto tgt = static_cast<GLenum> (_info.target);

        glBindBuffer(tgt, _handle);
        glBufferData(tgt, _info.initialData.size, nullptr, static_cast<GLenum> (_info.usage));
    }

    void buffer::bind(buffer_target target) const noexcept {
        glBindBuffer(static_cast<GLenum> (target), _handle);        
    }

    void buffer::subData(long offset, const void * src, std::size_t size) const noexcept {
        auto tgt = static_cast<GLenum> (_info.target);

        glBindBuffer(tgt, _handle);
        glBufferSubData(tgt, offset, size, src);
    }

    void buffer::bindBase(unsigned int index) const noexcept {
        glBindBufferBase(static_cast<GLenum> (_info.target), index, _handle);
    }

    void buffer::bindRange(unsigned int index, long offset, std::size_t size) const noexcept {
        glBindBufferRange(static_cast<GLenum> (_info.target), index, _handle, offset, size);
    }
}

#endif