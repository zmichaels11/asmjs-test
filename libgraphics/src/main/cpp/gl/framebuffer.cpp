#if defined(GL)
#include "pch.h"
#include "graphics/hobject/framebuffer.hpp"

#include "graphics/henum/draw_buffer.hpp"
#include "graphics/henum/internal_format.hpp"
#include "graphics/hobject/renderbuffer.hpp"
#include "graphics/hobject/texture.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;

        bool _isStencil(internal_format format) noexcept;

        bool _isDepthStencil(internal_format format) noexcept;

        bool _isDepth(internal_format format) noexcept;
    }

    framebuffer::framebuffer(const framebuffer_info& info) noexcept {
        _info = info;
        _external = false;
        _handle = 0;

        if (GLEW_VERSION_4_5) {
            glCreateFramebuffers(1, &_handle);
        } else {
            glGenFramebuffers(1, &_handle);
            glBindFramebuffer(GL_FRAMEBUFFER, _handle);
        }

        auto colorAttachId = GL_COLOR_ATTACHMENT0;

        for (auto it = info.pAttachments; it != (info.pAttachments + info.nAttachments); it++) {
            switch (it->type) {
                case attachment_type::RENDERBUFFER: {
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

                    if (GLEW_VERSION_4_5) {
                        glNamedFramebufferRenderbuffer(_handle, attachId, GL_RENDERBUFFER, it->pRenderbuffer->_handle);
                    } else {
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachId, GL_RENDERBUFFER, it->pRenderbuffer->_handle);
                    }
                } break;
                case attachment_type::TEXTURE: {
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

                    if (GLEW_VERSION_4_5) {
                        glNamedFramebufferTexture(_handle, attachId, it->pTexture->_handle, it->level);
                    } else {
                        glFramebufferTexture2D(GL_FRAMEBUFFER, attachId, GL_TEXTURE_2D, it->pTexture->_handle, it->level);
                    }
                } break;
                default:
                    _onError("Unknown attachment type!");
                    break;
            }
        }

        if (glCheckNamedFramebufferStatus(_handle, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            _onError("Incomplete framebuffer!");
        }

        _name = std::to_string(_handle);
    }

    framebuffer::~framebuffer() noexcept {
        if (_handle && !_external) {
            glDeleteFramebuffers(1, &_handle);
            _handle = 0;
        }
    }

    void framebuffer::setName(const std::string& name) noexcept {
        _name = name;

        if (GLEW_VERSION_4_3) {
            glObjectLabel(GL_FRAMEBUFFER, _handle, _name.size(), _name.c_str());
        }
    }

    const std::string& framebuffer::getName() const noexcept {
        return _name;
    }

    void framebuffer::bind() const noexcept {
        glBindFramebuffer(GL_FRAMEBUFFER, _handle);        
    }

    const framebuffer_info& framebuffer::getInfo() const noexcept {
        return _info;
    }

    void framebuffer::drawBuffers(const draw_buffer * pBuffers, std::size_t count) noexcept {        
        glDrawBuffers(static_cast<GLsizei> (count), reinterpret_cast<const GLenum * > (pBuffers));        
    }

    void framebuffer::readPixels(int x, int y, std::size_t width, std::size_t height, pixel_info& info) noexcept {
        glReadPixels(x, y, width, height, static_cast<GLenum> (info.format), static_cast<GLenum> (info.type), info.pData);
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[GL] framebuffer error: " << msg << std::endl;
            __builtin_trap();
        }    

        bool _isStencil(internal_format format) noexcept {
            switch (format) {
                case internal_format::STENCIL_INDEX8:
                    return true;
                default:
                    return false;
            }
        }

        bool _isDepthStencil(internal_format format) noexcept {
            switch (format) {
                case internal_format::DEPTH24_STENCIL8:
                case internal_format::DEPTH32F_STENCIL8:
                    return true;
                default:
                    return false;
            }
        }

        bool _isDepth(internal_format format) noexcept {
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
