#if defined(GLES30)
#include "pch.h"
#include "graphics/hobject/renderbuffer.hpp"

#include "graphics/hinfo/renderbuffer_info.hpp"

namespace graphics {
    renderbuffer::renderbuffer(const renderbuffer_info& info) noexcept {
        _info = info;
        _handle = 0;
        _external = false;

        glGenRenderbuffers(1, &_handle);

        glBindRenderbuffer(GL_RENDERBUFFER, _handle);
        glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum> (info.format), info.width, info.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        _name = std::to_string(_handle);
    }

    renderbuffer::~renderbuffer() noexcept {
        if (!_handle && _external) {
            glDeleteRenderbuffers(1, &_handle);
            _handle = 0;
        }
    }

    void renderbuffer::setName(const std::string& name) noexcept {
        _name = name;
    }

    const std::string& renderbuffer::getName() const noexcept {
        return _name;
    }
}
#endif
