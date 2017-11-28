#include "graphics/framebuffer.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <string>

#include "graphics/attachment_info.hpp"
#include "graphics/renderbuffer.hpp"
#include "graphics/internal_format.hpp"
#include "graphics/texture.hpp"

namespace graphics {
    namespace {    
        static void _onError(const std::string& msg) {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }    

        static bool _isStencil(internal_format format) {
            switch (format) {
                case internal_format::STENCIL_INDEX8:
                    return true;
                default:
                    return false;
            }
        }

        static bool _isDepthStencil(internal_format format) {
            switch (format) {
                case internal_format::DEPTH24_STENCIL8:
                case internal_format::DEPTH32F_STENCIL8:
                    return true;
                default:
                    return false;
            }
        }

        static bool _isDepth(internal_format format) {
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

    framebuffer::framebuffer(const framebuffer_info& info) {
        _info = info;

        glGenFramebuffers(1, &_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, _handle);

        GLenum colorAttachId = GL_COLOR_ATTACHMENT0;

        for (auto it = info.pAttachments; it != (info.pAttachments + info.attachmentCount); it++) {
            if (it->pRenderbuffer != nullptr) {
                const auto rbInfo = it->pRenderbuffer->getInfo();                
                GLenum attachId;                

                if (_isDepth(rbInfo.format)) {
                    attachId = GL_DEPTH_ATTACHMENT;
                } else if (_isDepthStencil(rbInfo.format)) {
                    attachId = GL_DEPTH_STENCIL_ATTACHMENT;
                } else if (_isStencil(rbInfo.format)) {
                    attachId = GL_STENCIL;
                } else {
                    attachId = colorAttachId++;
                }

                glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachId, GL_RENDERBUFFER, it->pRenderbuffer->_handle);
            } else if (it->pTexture != nullptr) {
                const auto txInfo = it->pTexture->getInfo();
                GLenum attachId;

                if (_isDepth(txInfo.format)) {
                    attachId = GL_DEPTH_ATTACHMENT;
                } else if (_isDepthStencil(txInfo.format)) {
                    attachId = GL_DEPTH_STENCIL_ATTACHMENT;
                } else if (_isStencil(txInfo.format)) {
                    attachId = GL_STENCIL_ATTACHMENT;
                } else {
                    attachId = colorAttachId++;
                }

                glFramebufferTexture2D(GL_FRAMEBUFFER, attachId, GL_TEXTURE_2D, it->pTexture->_handle, it->level);
            }
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            _onError("Incomplete framebuffer!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
}