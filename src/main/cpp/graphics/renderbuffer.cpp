#include "graphics/renderbuffer.hpp"

#include <GLES3/gl3.h>

#include "graphics/renderbuffer_info.hpp"

namespace graphics {
    renderbuffer::renderbuffer(const renderbuffer_info& info) {
        _info = info;

        glGenRenderbuffers(1, &_handle);

        glBindRenderbuffer(GL_RENDERBUFFER, _handle);
        glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum> (info.format), info.width, info.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    renderbuffer::~renderbuffer() {
        glDeleteRenderbuffers(1, &_handle);
    }

    const renderbuffer_info& renderbuffer::getInfo() const {
        return _info;
    }
}