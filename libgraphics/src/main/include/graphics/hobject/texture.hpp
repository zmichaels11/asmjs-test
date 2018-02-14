#pragma once

#include <string>
#include <utility>

#include "graphics/henum/texture_target.hpp"
#include "graphics/hinfo/pixel_info.hpp"
#include "graphics/hinfo/texture_info.hpp"
#include "graphics/hobject/image.hpp"

namespace graphics {
    class framebuffer;
    
    class texture {
        unsigned int _handle;
        graphics::texture_info _info;
        unsigned int _target;
        bool _external;
        std::string _name;

        texture(const texture&) = delete;

        texture& operator=(const texture&) = delete;

        friend class framebuffer;

    public:
        inline texture(texture&& other) noexcept {
            _info = std::move(other._info);
            _name = std::move(other._name);
            _handle = other._handle;
            _target = other._target;
            _external = other._external;

            other._handle = 0;
        }

        inline texture& operator=(texture&& other) {
            std::swap(_info, other._info);
            std::swap(_name, other._name);
            std::swap(_handle, other._handle);
            std::swap(_target, other._target);
            std::swap(_external, other._external);

            return *this;
        }

        texture() noexcept: 
            _handle(0),
            _info(),
            _target(0),
            _external(false),
            _name("0") {}
        
        texture(const texture_info& info) noexcept;

        texture(unsigned int handle, texture_target target) noexcept:
            _handle(handle),
            _info(),
            _target(static_cast<unsigned int> (target)),
            _external(true),
            _name(std::to_string(handle)) {}

        ~texture() noexcept;

        const graphics::texture_info& getInfo() const noexcept;

        void generateMipmaps() const noexcept;

        void bind(unsigned int unit) const noexcept;

        void subImage(unsigned int level, int x, int y, int z, unsigned int w, unsigned int h, unsigned int d, const pixel_info& px) const noexcept;

        void subImage(unsigned int level, int x, int y, int z, const image * pImage) const noexcept;        

        inline operator unsigned int() const noexcept {
            return _handle;
        }

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;
    };
}