#pragma once

#include "graphics/henum/texture_target.hpp"
#include "graphics/hinfo/pixel_info.hpp"
#include "graphics/hinfo/texture_info.hpp"

namespace graphics {
    class framebuffer;
    
    class texture {
        unsigned int _handle;
        graphics::texture_info _info;
        unsigned int _target;
        bool _external;

        texture(const texture&) = delete;

        texture& operator=(const texture&) = delete;

        friend class framebuffer;

    public:
        texture(texture&&) = default;

        texture& operator=(texture&&) = default;

        texture() noexcept: 
            _handle(0),
            _info(),
            _target(0),
            _external(false) {}
        
        texture(const texture_info& info) noexcept;

        texture(unsigned int handle, texture_target target) noexcept:
            _handle(handle),
            _info(),
            _target(static_cast<unsigned int> (target)),
            _external(true) {}

        ~texture() noexcept;        

        const graphics::texture_info& getInfo() const noexcept;

        void generateMipmap() const noexcept;

        void bind(unsigned int unit) const noexcept;

        void subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const noexcept;

        inline operator int() const noexcept {
            return _handle;
        }
    };
}