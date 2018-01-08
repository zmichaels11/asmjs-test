#include "graphics/hobject/buffered_image.hpp"

#include <cstddef>
#include <cstring>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/henum/pixel_format.hpp"
#include "graphics/hobject/image.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) noexcept {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }
    }

    buffered_image::buffered_image(unsigned int width, unsigned int height, pixel_format format) noexcept {
        _width = width;
        _height = height;
        _format = format;

        switch (format) {
            case pixel_format::RED:                            
                _pixelSize = 1;
                break;
            case pixel_format::RG:
                _pixelSize = 2;
                break;
            case pixel_format::RGB:
                _pixelSize = 3;
                break;
            case pixel_format::RGBA:
                _pixelSize = 4;
                break;
            default:
                _onError("Unsupported pixel format");
                break;
        }

        _dataSize = _pixelSize * width * height;
        _data = std::make_unique<unsigned char[]> (_dataSize);
    }

    unsigned int buffered_image::getWidth() const noexcept {
        return _width;
    }

    unsigned int buffered_image::getHeight() const noexcept {
        return _height;
    }

    const void * buffered_image::getData() const noexcept {
        return _data.get();
    }

    std::size_t buffered_image::getSize() const noexcept {
        return _dataSize;
    }

    void buffered_image::setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimage) noexcept {
        auto scale = _pixelSize;
        auto lineSize = w * scale;
        auto src = reinterpret_cast<const unsigned char *> (subimage.getData());
        auto dst = _data.get();

        for (int i = 0; i < h; i++) {
            auto srcOff = i * subimage.getWidth();
            auto dstOff = (y + i) * getWidth() + x;
            auto lineSrc = src + srcOff * scale;
            auto lineDst = dst + dstOff * scale;

            std::memcpy(lineDst, lineSrc, lineSize);
        }
    }

    pixel_format buffered_image::getFormat() const noexcept {
        return _format;
    }
}