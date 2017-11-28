#include "graphics/buffer.hpp"

#include <GLES3/gl3.h>

#include <cstddef>

#include "graphics/buffer_info.hpp"
#include "graphics/buffer_target.hpp"

namespace graphics {
    buffer::buffer(const buffer_info& info) {
        _info = info;
        
        glGenBuffers(1, &_handle);

        const auto tgt = static_cast<GLenum> (info.target);
        const auto usage = static_cast<GLenum> (info.usage);

        glBindBuffer(tgt, _handle);
        glBufferData(tgt, info.initialData.size, info.initialData.pData, usage);
    }

    buffer::~buffer() {
        glDeleteBuffers(1, &_handle);
    }

    void buffer::bind(buffer_target target) const {
        glBindBuffer(static_cast<GLenum> (target), _handle);        
    }

    void buffer::subData(long offset, const void * src, std::size_t size) const {
        const auto tgt = static_cast<GLenum> (_info.target);

        glBindBuffer(tgt, _handle);
        glBufferSubData(tgt, offset, size, src);
    }

    void buffer::bindBase(unsigned int index) const {
        glBindBufferBase(static_cast<GLenum> (_info.target), index, _handle);
    }

    void buffer::bindRange(unsigned int index, long offset, std::size_t size) const {
        glBindBufferRange(static_cast<GLenum> (_info.target), index, _handle, offset, size);
    }

    const buffer_info& buffer::getInfo() const {
        return _info;
    }
}