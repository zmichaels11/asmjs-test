#pragma once

#include <cstddef>

#include <memory>
#include <string>

#include "graphics/henum/pixel_format.hpp"

namespace graphics {
    class image {
    public:
        virtual ~image() noexcept {}
        
        virtual unsigned int getWidth() const noexcept = 0;

        virtual unsigned int getHeight() const noexcept = 0;

        virtual const void * getData() const noexcept = 0;

        virtual std::size_t getSize() const noexcept = 0;

        virtual graphics::pixel_format getFormat() const noexcept = 0;

        virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const graphics::image& subimg) noexcept = 0;

        static std::unique_ptr<image> read(const std::string& path, unsigned int forcedChannels = 0) noexcept;
    };
}