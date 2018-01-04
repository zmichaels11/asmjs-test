#pragma once

#include "graphics/framebuffer_info.hpp"
#include "graphics/pixel_info.hpp"

namespace graphics {
    class framebuffer {
        unsigned int _handle;
        framebuffer_info _info;
        bool _external;

        framebuffer(const framebuffer&) = delete;

        framebuffer& operator=(const framebuffer&) = delete;

    public:
        framebuffer() : 
            _handle(0),
            _info(),
            _external(false) {}

        framebuffer(const framebuffer_info& info);

        framebuffer(unsigned int handle) :
            _handle(handle),
            _info(),
            _external(true) {}

        framebuffer(framebuffer&&) = default;

        ~framebuffer();

        framebuffer& operator=(framebuffer&&) = default;

        const graphics::framebuffer_info& getInfo() const;

        void bind() const;

        static void readPixels(int x, int y, std::size_t width, std::size_t height, graphics::pixel_info& info);
    };
}