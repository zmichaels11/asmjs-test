#include "graphics/texture.hpp"

#include <GLES3/gl3.h>

#include <iostream>
#include <string>

#include "graphics/pixel_info.hpp"
#include "graphics/texture_info.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << msg << std::endl;
            __builtin_trap();
        }
    }

    texture::texture(const texture_info& info) {
        glGenTextures(1, &_handle);
        
        const auto internalFormat = static_cast<GLenum> (info.format);

        if (info.extent.height > 1) {
            if (info.extent.depth > 1) {
                _target = GL_TEXTURE_3D;
                glBindTexture(GL_TEXTURE_3D, _handle);
                glTexStorage3D(GL_TEXTURE_3D, info.levels, internalFormat, info.extent.width, info.extent.height, info.extent.depth);
            } else if (info.layers > 1) {
                _target = GL_TEXTURE_2D_ARRAY;
                glBindTexture(GL_TEXTURE_2D_ARRAY, _handle);
                glTexStorage3D(GL_TEXTURE_2D_ARRAY, info.levels, internalFormat, info.extent.width, info.extent.height, info.layers);
            } else {
                _target = GL_TEXTURE_2D;
                glBindTexture(GL_TEXTURE_2D, _handle);
                glTexStorage2D(GL_TEXTURE_2D, info.levels, internalFormat, info.extent.width, info.extent.height);
            }
        } else if (info.layers > 1) {
            _target = GL_TEXTURE_2D;
            glBindTexture(GL_TEXTURE_2D, _handle);
            glTexStorage2D(GL_TEXTURE_2D, info.levels, internalFormat, info.extent.width, info.layers);
        } else {
            _target = GL_TEXTURE_2D;
            glBindTexture(GL_TEXTURE_2D, _handle);
            glTexStorage2D(GL_TEXTURE_2D, info.levels, internalFormat, info.extent.width, 1);
        }        
    }

    texture::~texture() {
        glDeleteTextures(1, &_handle);
    }

    const texture_info& texture::getInfo() const {
        return _info;
    }

    void texture::generateMipmap() const {
        glBindTexture(_target, _handle);
        glGenerateMipmap(_target);
    }

    void texture::bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(_target, _handle);
    }

    void texture::subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const {
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
}