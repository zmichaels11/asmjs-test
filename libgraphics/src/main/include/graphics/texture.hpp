#pragma once

#include "graphics/pixel_info.hpp"
#include "graphics/texture_info.hpp"

namespace graphics {
    class framebuffer;

    class texture {
        unsigned int _handle;
        texture_info _info;
        unsigned int _target;

        texture(const texture&) = delete;

        texture& operator=(const texture&) = delete;

        friend class framebuffer;

    public:
        texture() : _handle(0) {}
        
        texture(const texture_info& info);

        texture(texture&&) = default;

        ~texture();

        texture& operator=(texture&&) = default;

        const texture_info& getInfo() const;

        void generateMipmap() const;

        void bind(unsigned int unit) const;

        void subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const;        

        inline operator int() const {
            return _handle;
        }
    };
}