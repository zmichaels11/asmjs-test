#ifdef GL

#include "graphics/renderbuffer.hpp"

#include "GL/glew.h"

#include "graphics/renderbuffer_info.hpp"

namespace graphics {
    renderbuffer::renderbuffer(const renderbuffer_info& info) {        
        _info = info;
        _external = false;
        _handle = 0;

        if (GLEW_VERSION_4_5) {
            glCreateRenderbuffers(1, &_handle);        
            glNamedRenderbufferStorage(_handle, static_cast<GLenum> (info.format), info.width, info.height);
        } else {
            glGenRenderbuffers(1, &_handle);
            glBindRenderbuffer(GL_RENDERBUFFER, _handle);
            glRenderbufferStorage(_handle, static_cast<GLenum> (info.format), info.width, info.height);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }
    }

    renderbuffer::~renderbuffer() {
        if (_handle && !_external) {
            glDeleteRenderbuffers(1, &_handle);
            _handle = 0;
        }
    }
}

#endif