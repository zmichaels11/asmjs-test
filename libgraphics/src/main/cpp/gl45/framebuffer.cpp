#ifdef GL45

#include "graphics/framebuffer.hpp"

#include <GL/glew.h>

#include <iostream>
#include <string>

#include "graphics/internal_format.hpp"
#include "graphics/renderbuffer.hpp"
#include "graphics/texture.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg);

        bool _isStencil(internal_format format);

        bool _isDepthStencil(internal_format format);

        bool _isDepth(internal_format format);
    }

    framebuffer::framebuffer(const framebuffer_info& info) {
        _info = info;

        glCreateFramebuffers(1, &_handle);

        auto colorAttachId = GL_COLOR_ATTACHMENT0;

        for (auto it = info.pAttachments; it != (info.pAttachments + info.attachmentCount); it++) {
            if (it->pRenderbuffer != nullptr) {
                auto rbInfo = it->pRenderbuffer->getInfo();                
                decltype(colorAttachId) attachId;                

                if (_isDepth(rbInfo.format)) {
                    attachId = GL_DEPTH_ATTACHMENT;
                } else if (_isDepthStencil(rbInfo.format)) {
                    attachId = GL_DEPTH_STENCIL_ATTACHMENT;
                } else if (_isStencil(rbInfo.format)) {
                    attachId = GL_STENCIL;
                } else {
                    attachId = colorAttachId++;
                }

                glNamedFramebufferRenderbuffer(_handle, attachId, GL_RENDERBUFFER, it->pRenderbuffer->_handle);
            } else if (it->pTexture != nullptr) {
                auto txInfo = it->pTexture->getInfo();
                decltype(colorAttachId) attachId;

                if (_isDepth(txInfo.format)) {
                    attachId = GL_DEPTH_ATTACHMENT;
                } else if (_isDepthStencil(txInfo.format)) {
                    attachId = GL_DEPTH_STENCIL_ATTACHMENT;
                } else if (_isStencil(txInfo.format)) {
                    attachId = GL_STENCIL_ATTACHMENT;
                } else {
                    attachId = colorAttachId++;
                }

                glNamedFramebufferTexture(_handle, attachId, it->pTexture->_handle, it->level);
            }
        }

        if (glCheckNamedFramebufferStatus(_handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            _onError("Incomplete framebuffer!");
        }
    }

    framebuffer::~framebuffer() {
        glDeleteFramebuffers(1, &_handle);
    }

    void framebuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, _handle);
    }

    const framebuffer_info& framebuffer::getInfo() const {
        return _info;
    }

    void framebuffer::readPixels(int x, int y, std::size_t width, std::size_t height, pixel_info& info) {        
        glReadPixels(x, y, width, height, static_cast<GLenum> (info.format), static_cast<GLenum> (info.type), info.pData);
    }

    namespace {
        void _onError(const std::string& msg) {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }    

        bool _isStencil(internal_format format) {
            switch (format) {
                case internal_format::STENCIL_INDEX8:
                    return true;
                default:
                    return false;
            }
        }

        bool _isDepthStencil(internal_format format) {
            switch (format) {
                case internal_format::DEPTH24_STENCIL8:
                case internal_format::DEPTH32F_STENCIL8:
                    return true;
                default:
                    return false;
            }
        }

        bool _isDepth(internal_format format) {
            switch (format) {
                case internal_format::DEPTH_COMPONENT16:
                case internal_format::DEPTH_COMPONENT24:
                case internal_format::DEPTH_COMPONENT32F:
                    return true;
                default:
                    return false;
            }
        }   
    }
}
#endif