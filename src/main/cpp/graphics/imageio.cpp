#include "graphics/imageio.hpp"

#include <cstddef>
#include <cstring>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/pixel_format.hpp"
#include "graphics/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#define STB_IMAGE_STATIC
#include "stbimage.cpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
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
            stb_image(const std::string& file, unsigned int forcedComponents) {
                int width, height, channels;

                _data = stbi_load(file.c_str(), &width, &height, &channels, forcedComponents);
                _width = width;
                _height = height;

                unsigned int npixels = _width * _height;

                switch (channels) {
                    case 1:
                        _pixelSize = 1;
                        _dataSize = npixels;
                        _format = pixel_format::RED;
                        break;
                    case 2:
                        _pixelSize = 2;
                        _dataSize = npixels * 2;
                        _format = pixel_format::RG;
                        break;
                    case 3:
                        _pixelSize = 3;
                        _dataSize = npixels * 3;
                        _format = pixel_format::RGB;
                        break;
                    case 4:
                        _pixelSize = 4;
                        _dataSize = npixels * 4;
                        _format = pixel_format::RGBA;
                        break;
                    default:
                        _onError("Unsupported pixel format!");
                        break;
                }                                                
            }

            ~stb_image() {
                stbi_image_free(_data);
            }

            virtual unsigned int getWidth() const {
                return _width;
            }            

            virtual unsigned int getHeight() const {
                return _height;
            }

            virtual const void * getData() const {
                return _data;
            }

            virtual std::size_t getSize() const {
                return _dataSize;
            }

            virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimage) {
                const auto scale = _pixelSize;
                const auto lineSize = w * scale;

                auto src = reinterpret_cast<const unsigned char *> (subimage.getData());
                auto dst = reinterpret_cast<unsigned char *> (_data);

                for (int i = 0; i < h; i++) {
                    const auto srcOff = i * subimage.getWidth();
                    const auto dstOff = (y + i) * getWidth() + x;
                    const auto lineSrc = src + srcOff * scale;
                    const auto lineDst = dst + dstOff * scale;

                    std::memcpy(lineDst, lineSrc, lineSize);
                }
            }

            virtual pixel_format getFormat() const {
                return _format;
            }
        };
    }

    namespace image_io {
        std::unique_ptr<image> read(const std::string& path, unsigned int forcedChannels) {
            return std::make_unique<stb_image> (path, forcedChannels);
        }
    }
}