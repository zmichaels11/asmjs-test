#include <cstddef>
#include <cstring>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/henum/pixel_format.hpp"
#include "graphics/hobject/image.hpp"

#define STB_IMAGE_IMPLEMENTATION

#ifdef __EMSCRIPTEN__
#define STBI_NO_SIMD
#endif

#define STBI_ONLY_PNG
#define STB_IMAGE_STATIC
#include "stb_image.h"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) noexcept {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }

        class stb_image : public image{
            unsigned int _width;
            unsigned int _height;
            unsigned char * _data;
            std::size_t _dataSize;
            pixel_format _format;
            unsigned int _pixelSize;

            stb_image(const stb_image&) = delete;

            stb_image& operator=(const stb_image&) = delete;
        
        public:
            stb_image(const std::string& file, unsigned int forcedComponents) noexcept {
                int width, height, channels;

                _data = stbi_load(file.c_str(), &width, &height, &channels, forcedComponents);
                _width = static_cast<decltype(_width)> (width);
                _height = static_cast<decltype(_height)> (height);

                auto npixels = _width * _height;

                switch (channels) {
                    case 1:
                        _pixelSize = 1;
                        _dataSize = static_cast<decltype(_dataSize)> (npixels);
                        _format = pixel_format::RED;
                        break;
                    case 2:
                        _pixelSize = 2;
                        _dataSize = static_cast<decltype(_dataSize)> (npixels * 2);
                        _format = pixel_format::RG;
                        break;
                    case 3:
                        _pixelSize = 3;
                        _dataSize = static_cast<decltype(_dataSize)> (npixels * 3);
                        _format = pixel_format::RGB;
                        break;
                    case 4:
                        _pixelSize = 4;
                        _dataSize = static_cast<decltype(_dataSize)> (npixels * 4);
                        _format = pixel_format::RGBA;
                        break;
                    default:
                        _onError("Unsupported pixel format!");
                        break;
                }                                                
            }

            ~stb_image() noexcept {
                stbi_image_free(_data);
            }

            virtual unsigned int getWidth() const noexcept {
                return _width;
            }            

            virtual unsigned int getHeight() const noexcept {
                return _height;
            }

            virtual const void * getData() const noexcept {
                return _data;
            }

            virtual std::size_t getSize() const noexcept {
                return _dataSize;
            }

            virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimage) noexcept {
                auto scale = _pixelSize;
                auto lineSize = w * scale;
                auto src = reinterpret_cast<const unsigned char *> (subimage.getData());
                auto dst = reinterpret_cast<unsigned char *> (_data);

                for (int i = 0; i < h; i++) {
                    auto srcOff = i * subimage.getWidth();
                    auto dstOff = (y + i) * getWidth() + x;
                    auto lineSrc = src + srcOff * scale;
                    auto lineDst = dst + dstOff * scale;

                    std::memcpy(lineDst, lineSrc, lineSize);
                }
            }

            virtual pixel_format getFormat() const noexcept {
                return _format;
            }
        };
    }

    std::unique_ptr<image> image::read(const std::string& path, unsigned int forcedChannels) noexcept {
        return std::make_unique<stb_image> (path, forcedChannels);
    }    
}