#ifdef GL

#include "graphics/hobject/buffer.hpp"

#include "GL/glew.h"

#include <iostream>

#include "graphics/henum/buffer_target.hpp"
#include "graphics/hinfo/buffer_info.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    buffer::buffer(const buffer_info& info) noexcept {
        _info = info;
        _external = false;
        _handle = 0;
        
        auto usage = static_cast<GLenum> (info.usage);

        if (GLEW_VERSION_4_5) {
            glCreateBuffers(1, &_handle);
            glNamedBufferData(_handle, info.initialData.size, info.initialData.pData, usage);
        } else {
            auto tgt = static_cast<GLenum> (info.target);

            glGenBuffers(1, &_handle);
            glBindBuffer(tgt, _handle);
            glBufferData(tgt, info.initialData.size, info.initialData.pData, usage);
            glBindBuffer(tgt, 0);
        }
    }

    buffer::~buffer() noexcept {
        if (_handle && !_external) {
            glDeleteBuffers(1, &_handle);            
        }
    }

    void buffer::invalidate() const noexcept {
        if (GLEW_VERSION_4_5) {
            glInvalidateBufferData(_handle);
        } else {
            auto tgt = static_cast<GLenum> (_info.target);
            
            glBindBuffer(tgt, _handle);
            glBufferData(tgt, _info.initialData.size, nullptr, static_cast<GLenum> (_info.usage));
            glBindBuffer(tgt, 0);
        }
    }

    void buffer::bind(buffer_target target) const noexcept {
        glBindBuffer(static_cast<GLenum> (target), _handle);        
    }

    void buffer::subData(long offset, const void * src, std::size_t size) const noexcept{        
        if (GLEW_VERSION_4_5) {
            glNamedBufferSubData(_handle, offset, size, src);
        } else {
            auto tgt = static_cast<GLenum> (_info.target);

            glBindBuffer(tgt, _handle);
            glBufferData(tgt, _info.initialData.size, nullptr, static_cast<GLenum> (_info.usage));
            glBindBuffer(tgt, 0);
        }
    }

    void buffer::bindBase(unsigned int index) const noexcept {
        glBindBufferBase(static_cast<GLenum> (_info.target), index, _handle);
    }

    void buffer::bindRange(unsigned int index, long offset, std::size_t size) const noexcept {
        glBindBufferRange(static_cast<GLenum> (_info.target), index, _handle, offset, size);
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }
    }
}

#endif