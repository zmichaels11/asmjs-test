#pragma once

#include <cstddef>

#include <memory>

#include "graphics/henum/pixel_format.hpp"
#include "graphics/henum/pixel_type.hpp"
#include "graphics/hobject/image.hpp"

namespace graphics {
    class buffered_image : public image {
        std::unique_ptr<unsigned char[]> _data;
        std::size_t _dataSize;

        unsigned int _width;
        unsigned int _height;
        graphics::pixel_format _format;
        graphics::pixel_type _type;
        unsigned int _pixelSize;

        buffered_image(const buffered_image&) = delete;

        buffered_image& operator=(const buffered_image&) = delete;

    public:
        buffered_image(buffered_image&&) noexcept = default;

        buffered_image& operator=(buffered_image&&) noexcept = default;

        buffered_image(
            unsigned int width, unsigned int height, 
            pixel_format format = pixel_format::RGBA,
            pixel_type type = pixel_type::UNSIGNED_BYTE) noexcept;

        virtual ~buffered_image() {}        

        virtual unsigned int getWidth() const noexcept;

        virtual unsigned int getHeight() const noexcept;

        virtual const void * getData() const noexcept;

        virtual std::size_t getSize() const noexcept;

        virtual graphics::pixel_type getType() const noexcept;

        virtual graphics::pixel_format getFormat() const noexcept;

        virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimg) noexcept;
    };
}