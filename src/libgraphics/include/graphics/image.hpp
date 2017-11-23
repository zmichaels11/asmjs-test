#pragma once

#include <cstddef>

#include "graphics/pixel_format.hpp"

namespace graphics {
    class image {
    public:
        virtual ~image() {}
        
        virtual unsigned int getWidth() const = 0;

        virtual unsigned int getHeight() const = 0;

        virtual const void * getData() const = 0;

        virtual std::size_t getSize() const = 0;

        virtual pixel_format getFormat() const = 0;

        virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimg) = 0;
    };
}