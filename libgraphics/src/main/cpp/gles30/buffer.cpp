#if defined(GLES30)
#include "pch.h"
#include "graphics/hobject/buffer.hpp"

#include "graphics/henum/buffer_target.hpp"
#include "graphics/hinfo/buffer_info.hpp"

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
        glBindBuffer(tgt, 0);

        _name = std::to_string(_handle);
    }

    buffer::~buffer() noexcept {
        if (_handle && !_external) {
            glDeleteBuffers(1, &_handle);
            _handle = 0;
        }
    }

    void * buffer::map(long offset, std::size_t length, buffer_access access) const noexcept {       
        auto tgt = static_cast<GLenum> (_info.target);

        glBindBuffer(tgt, _handle);
        
        auto res = glMapBufferRange(tgt, static_cast<GLintptr> (offset), static_cast<GLsizeiptr> (length), static_cast<GLbitfield> (access));

        glBindBuffer(tgt, 0);
       
        return res;        
    }

    void buffer::unmap() const noexcept {        
        auto tgt = static_cast<GLenum> (_info.target);

        glBindBuffer(tgt, _handle);
        glUnmapBuffer(tgt);
        glBindBuffer(tgt, 0);        
    }

    void buffer::setName(const std::string& name) noexcept {
        _name = name;
    }

    const std::string& buffer::getName() const noexcept {
        return _name;
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
