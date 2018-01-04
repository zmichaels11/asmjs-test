#pragma once

#include "graphics/framebuffer.hpp"
#include "graphics/pixel_info.hpp"
#include "graphics/texture_info.hpp"
#include "graphics/texture_target.hpp"

namespace graphics {
    class texture {
        unsigned int _handle;
        graphics::texture_info _info;
        unsigned int _target;
        bool _external;

        texture(const texture&) = delete;

        texture& operator=(const texture&) = delete;

        friend class framebuffer;

    public:
        texture() : 
            _handle(0),
            _info(),
            _target(0),
            _external(false) {}
        
        texture(const texture_info& info);

        texture(unsigned int handle, texture_target target) :
            _handle(handle),
            _info(),
            _target(static_cast<unsigned int> (target)),
            _external(true) {}

        texture(texture&&) = default;

        ~texture();

        texture& operator=(texture&&) = default;

        const graphics::texture_info& getInfo() const;

        void generateMipmap() const;

        void bind(unsigned int unit) const;

        void subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const;        

        inline operator int() const {
            return _handle;
        }
    };
}