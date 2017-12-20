#ifdef GL45

#include "graphics/renderbuffer.hpp"

#include <GL/glew.h>

#include "graphics/renderbuffer_info.hpp"

namespace graphics {
    renderbuffer::renderbuffer(const renderbuffer_info& info) {        
        _info = info;

        glCreateRenderbuffers(1, &_handle);
        
        glNamedRenderbufferStorage(_handle, static_cast<GLenum> (info.format), info.width, info.height);
    }

    renderbuffer::~renderbuffer() {
        glDeleteRenderbuffers(1, &_handle);
    }
}

#endif