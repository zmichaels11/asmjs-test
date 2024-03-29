#if defined(GLES30)
#include "pch.h"
#include "graphics/hobject/texture.hpp"

#include "graphics/hinfo/pixel_info.hpp"
#include "graphics/hinfo/texture_info.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;

        GLenum _getTarget(const texture_info& info) noexcept;
    }

    texture::texture(const texture_info& info) noexcept {
        _info = info;
        _external = false;
        _handle = 0;
        
        glGenTextures(1, &_handle);
        
        auto internalFormat = static_cast<GLenum> (info.format);
        _target = _getTarget(info);

        glBindTexture(_target, _handle);

        glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, static_cast<GLenum> (info.samplerInfo.filters.mag));
        glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, static_cast<GLenum> (info.samplerInfo.filters.min));
        glTexParameteri(_target, GL_TEXTURE_WRAP_S, static_cast<GLenum> (info.samplerInfo.addressing.wrapS));
        glTexParameteri(_target, GL_TEXTURE_WRAP_T, static_cast<GLenum> (info.samplerInfo.addressing.wrapT));
        glTexParameteri(_target, GL_TEXTURE_WRAP_R, static_cast<GLenum> (info.samplerInfo.addressing.wrapR));
        glTexParameterf(_target, GL_TEXTURE_MAX_LOD, info.samplerInfo.lod.max);
        glTexParameterf(_target, GL_TEXTURE_MIN_LOD, info.samplerInfo.lod.min);

        switch (_target) {
            case GL_TEXTURE_3D:                
                glTexStorage3D(_target, info.levels, internalFormat, info.extent.width, info.extent.height, info.extent.depth);                
                break;
            case GL_TEXTURE_2D_ARRAY:
                glTexStorage3D(_target, info.levels, internalFormat, info.extent.width, info.extent.height, info.layers);
                break;
            case GL_TEXTURE_2D:
                glTexStorage2D(_target, info.levels, internalFormat, info.extent.width, info.extent.height);
                break;
            default:
                _onError("Unsupported texture target!");
        }

        glBindTexture(_target, 0);

        std::to_string(_handle);
    }

    texture::~texture() noexcept {
        if (_handle && !_external) {
            glDeleteTextures(1, &_handle);
            _handle = 0;
        }
    }

    void texture::setName(const std::string& name) noexcept {
        _name = name;
    }

    const std::string& texture::getName() const noexcept {
        return _name;
    }

    void texture::generateMipmaps() const noexcept {
        glBindTexture(_target, _handle);
        glGenerateMipmap(_target);
    }

    void texture::bind(unsigned int unit) const noexcept {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(_target, _handle);
    }

    void texture::subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const noexcept {
        glBindTexture(_target, _handle);

        switch (_target) {
            case GL_TEXTURE_2D:
                glTexSubImage2D(_target, level, x, y, w, h, static_cast<GLenum> (px.format), static_cast<GLenum> (px.type), px.pData);
                break;
            case GL_TEXTURE_2D_ARRAY:
            case GL_TEXTURE_3D:
                glTexSubImage3D(_target, level, x, y, z, w, h, d, static_cast<GLenum> (px.format), static_cast<GLenum> (px.type), px.pData);
                break;
        }        
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[GLES] texture error: " << msg << std::endl;
            __builtin_trap();
        }

        GLenum _getTarget(const texture_info& info) noexcept {
            if (info.extent.height > 1) {
                if (info.extent.depth > 1) {
                    return GL_TEXTURE_3D; 
                } else if (info.layers > 1) {
                    return GL_TEXTURE_2D_ARRAY;
                } else {
                    return GL_TEXTURE_2D;
                }
            } else {
                return GL_TEXTURE_2D;
            }
        }
    }
}
#endif
