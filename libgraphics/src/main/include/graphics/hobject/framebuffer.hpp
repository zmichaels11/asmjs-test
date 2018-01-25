#pragma once

#include <cstddef>

#include <string>

#include "graphics/henum/draw_buffer.hpp"
#include "graphics/hinfo/framebuffer_info.hpp"
#include "graphics/hinfo/pixel_info.hpp"

namespace graphics {
    class framebuffer {
        unsigned int _handle;
        framebuffer_info _info;
        bool _external;
        std::string _name;

        framebuffer(const framebuffer&) = delete;

        framebuffer& operator=(const framebuffer&) = delete;

    public:
        framebuffer(framebuffer&&) = default;

        framebuffer& operator=(framebuffer&&) = default;

        framebuffer() noexcept: 
            _handle(0),
            _info(),
            _external(false),
            _name("0") {}

        framebuffer(const framebuffer_info& info) noexcept;

        framebuffer(unsigned int handle) noexcept:
            _handle(handle),
            _info(),
            _external(true),
            _name(std::to_string(handle)) {}

        ~framebuffer() noexcept;        

        const graphics::framebuffer_info& getInfo() const noexcept;

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;

        void bind() const noexcept;
        
        static void drawBuffers(const draw_buffer * pBuffers, std::size_t count = 1) noexcept;

        static void readPixels(int x, int y, std::size_t width, std::size_t height, graphics::pixel_info& info) noexcept;

        static const framebuffer& getDefault() noexcept;
    };
}