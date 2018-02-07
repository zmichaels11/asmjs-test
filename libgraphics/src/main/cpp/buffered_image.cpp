#include "pch.h"
#include "graphics/hobject/buffered_image.hpp"

#include "graphics/henum/pixel_format.hpp"
#include "graphics/hobject/image.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;

        unsigned int _calcPixelSize(pixel_format fmt, pixel_type type) noexcept;
    }

    buffered_image::buffered_image(unsigned int width, unsigned int height, pixel_format format, pixel_type type) noexcept {
        _width = width;
        _height = height;
        _format = format;
        _type = type;
        _pixelSize = _calcPixelSize(format, type);
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

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[gfx] buffered_image error: " << msg << std::endl;
            __builtin_trap();
        }

        unsigned int _calcPixelSize(pixel_format fmt, pixel_type type) noexcept {
            switch (type) {
                case pixel_type::UNSIGNED_SHORT_5_6_5:
                    if (fmt == pixel_format::RGB) {
                        return 2;
                    } else {
                        _onError("Packed type [UNSIGNED_SHORT_5_6_5] requires RGB!");
                    }
                case pixel_type::UNSIGNED_SHORT_5_5_5_1:
                    if (fmt == pixel_format::RGBA) {
                        return 2;
                    } else {
                        _onError("Packed type [UNSIGNED_SHORT_5_5_5_1] requires RGBA!");
                    }
                case pixel_type::UNSIGNED_SHORT_4_4_4_4:
                    if (fmt == pixel_format::RGBA) {
                        return 2;
                    } else {
                        _onError("Packed type [UNSIGNED_SHORT_4_4_4_4] requires RGBA!");
                    }
                case pixel_type::UNSIGNED_INT_5_9_9_9_REV:
                    if (fmt == pixel_format::RGBA) {
                        return 4;
                    } else {
                        _onError("Packed type [UNSIGNED_INT_5_9_9_9_REV] requires RGBA!");
                    }
                case pixel_type::UNSIGNED_INT_2_10_10_10_REV:
                    if (fmt == pixel_format::RGBA) {
                        return 4;
                    } else {
                        _onError("Packed type [UNSIGNED_INT_2_10_10_10_REV] requires RGBA!");
                    }
                default:
                    // determine the packed size using normal means
                    break;
            }


            unsigned int size;

            switch (fmt) {
                case pixel_format::RED:
                case pixel_format::RED_INTEGER:
                    size = 1;
                    break;
                case pixel_format::RG:
                case pixel_format::RG_INTEGER:
                    size = 2;
                    break;
                case pixel_format::RGB:
                case pixel_format::RGB_INTEGER:                    
                    size = 3;
                    break;
                case pixel_format::RGBA:
                case pixel_format::RGBA_INTEGER:
                    size = 4;
                    break;
                default:
                    _onError("Unsupported pixel_format!");
            }

            switch (type) {
                case pixel_type::UNSIGNED_BYTE:
                case pixel_type::BYTE:
                    // size doesn't change
                    break;
                case pixel_type::UNSIGNED_SHORT:
                case pixel_type::SHORT:
                case pixel_type::HALF_FLOAT:
                    size *= 2;
                    break;
                case pixel_type::UNSIGNED_INT:
                case pixel_type::INT:
                case pixel_type::FLOAT:
                    size *= 4;
                    break;
                default:
                    _onError("Unsupported pixel_type!");
            }

            return size;
        }
    }
}
