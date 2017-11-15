#include "graphics/buffered_image.hpp"

#include <cstddef>
#include <cstring>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/pixel_format.hpp"
#include "graphics/image.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }
    }

    buffered_image::buffered_image(unsigned int width, unsigned int height, pixel_format format) {
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

    unsigned int buffered_image::getWidth() const {
        return _width;
    }

    unsigned int buffered_image::getHeight() const {
        return _height;
    }

    const void * buffered_image::getData() const {
        return _data.get();
    }

    std::size_t buffered_image::getSize() const {
        return _dataSize;
    }

    void buffered_image::setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimage) {
        const auto scale = _pixelSize;
        const auto lineSize = w * scale;

        auto src = reinterpret_cast<const unsigned char *> (subimage.getData());
        auto dst = _data.get();

        for (int i = 0; i < h; i++) {
            const auto srcOff = i * subimage.getWidth();
            const auto dstOff = (y + i) * getWidth() + x;
            const auto lineSrc = src + srcOff * scale;
            const auto lineDst = dst + dstOff * scale;

            std::memcpy(lineDst, lineSrc, lineSize);
        }
    }

    pixel_format buffered_image::getFormat() const {
        return _format;
    }
}