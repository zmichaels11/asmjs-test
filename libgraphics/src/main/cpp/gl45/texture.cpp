#ifdef GL45

#include "graphics/texture.hpp"

#include <GL/glew.h>

#include <iostream>
#include <string>

#include "graphics/pixel_info.hpp"
#include "graphics/texture_info.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg);

        GLenum _getTarget(const texture_info& info);        
    }

    texture::texture(const texture_info& info) {               
        auto internalFormat = static_cast<GLenum> (info.format);
        _target = _getTarget(info);   
        _handle = 0;

        glCreateTextures(_target, 1, &_handle);

        glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, static_cast<GLenum> (info.samplerInfo.filters.mag));
        glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, static_cast<GLenum> (info.samplerInfo.filters.min));
        glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, static_cast<GLenum> (info.samplerInfo.addressing.wrapS));
        glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, static_cast<GLenum> (info.samplerInfo.addressing.wrapT));
        glTextureParameteri(_handle, GL_TEXTURE_WRAP_R, static_cast<GLenum> (info.samplerInfo.addressing.wrapR));
        glTextureParameterf(_handle, GL_TEXTURE_MAX_LOD, info.samplerInfo.lod.max);
        glTextureParameterf(_handle, GL_TEXTURE_MIN_LOD, info.samplerInfo.lod.min);

        switch (_target) {
            case GL_TEXTURE_3D:
                glTextureStorage3D(_handle, info.levels, internalFormat, info.extent.width, info.extent.height, info.extent.depth);                
                break;
            case GL_TEXTURE_2D_ARRAY:
                glTextureStorage3D(_handle, info.levels, internalFormat, info.extent.width, info.extent.height, info.layers);
                break;
            case GL_TEXTURE_2D:
                glTextureStorage2D(_handle, info.levels, internalFormat, info.extent.width, info.extent.height);
                break;
            default:
                _onError("Unsupported texture target!");
        }        
    }

    texture::~texture() {
        if (_handle) {
            glDeleteTextures(1, &_handle);
        }
    }

    void texture::generateMipmap() const {
        glGenerateTextureMipmap(_handle);
    }

    void texture::bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(_target, _handle);
    }

    void texture::subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const {        
        switch (_target) {
            case GL_TEXTURE_2D:
                glTextureSubImage2D(_handle, level, x, y, w, h, static_cast<GLenum> (px.format), static_cast<GLenum> (px.type), px.pData);
                break;
            case GL_TEXTURE_2D_ARRAY:
            case GL_TEXTURE_3D:
                glTextureSubImage3D(_handle, level, x, y, z, w, h, d, static_cast<GLenum> (px.format), static_cast<GLenum> (px.type), px.pData);
                break;
        }
    }

    namespace {
        void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }

        GLenum _getTarget(const texture_info& info) {
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