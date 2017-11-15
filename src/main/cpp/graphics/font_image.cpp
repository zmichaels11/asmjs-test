#include "graphics/font_image.hpp"

#include <cstddef>

#include <iostream>
#include <memory>
#include <string>

#include "graphics/font_info.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.cpp"

#include "util.hpp"

namespace graphics {
    namespace {
        static void _onError(const std::string& msg) {
            std::cerr << "Err: " << msg << std::endl;
            __builtin_trap();
        }

        struct font_resources_impl : public font_resources {
            std::unique_ptr<stbtt_bakedchar[]> _cdata;
            stbtt_fontinfo _fontInfo;
            float _scale;            
            float _ascent, _descent, _lineGap, _lineSpacing;
            std::unique_ptr<unsigned char[]> _data;
            std::size_t _dataSize;
            unsigned int _width;
            unsigned int _height;

            font_resources_impl(const font_info& info) {
                const auto fontData = util::readAll(info.fontFile);

                stbtt_InitFont(&_fontInfo, reinterpret_cast<const unsigned char *> (fontData.get()), 0);
                _scale = stbtt_ScaleForPixelHeight(&_fontInfo, info.charHeight);

                int ascent, descent, lineGap;
                stbtt_GetFontVMetrics(&_fontInfo, &ascent, &descent, &lineGap);

                _ascent = _scale * ascent;
                _descent = _scale * descent;
                _lineGap = _scale * lineGap;
                _lineSpacing = _ascent - _descent + _lineGap;
                _cdata = std::make_unique<stbtt_bakedchar[]> (info.charCount);

                const auto neededArea = info.charCount * info.charHeight * info.charHeight;
                bool flipflop = false;
                int w = 1;
                int h = 1;

                while (w * h < neededArea) {
                    if (flipflop) {
                        w <<= 1;
                    } else {
                        h <<= 1;
                    }

                    flipflop = !flipflop;
                }
                 
                _dataSize = w * h;
                _data = std::make_unique<unsigned char[]> (_dataSize);

                while (stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char *> (fontData.get()), 0, info.charHeight, _data.get(), w, h, info.firstChar, info.charCount, _cdata.get()) == 0) {
                    if (flipflop) {
                        w <<= 1;
                    } else {
                        h <<= 1;
                    }

                    flipflop = !flipflop;
                    _dataSize = w * h;
                    _data = std::make_unique<unsigned char[]> (_dataSize);
                }

                _width = w;
                _height = h;
            }

            virtual ~font_resources_impl() {}
        };
    }

    font_image::font_image(const font_info& info) {        
        _resources = std::make_unique<font_resources_impl>(info);
    }

    unsigned int font_image::getWidth() const {
        auto res = reinterpret_cast<font_resources_impl *> (_resources.get());

        return res->_width;
    }

    unsigned int font_image::getHeight() const {
        auto res = reinterpret_cast<font_resources_impl *> (_resources.get());

        return res->_height;
    }

    const void * font_image::getData() const {
        auto res = reinterpret_cast<font_resources_impl *> (_resources.get());

        return res->_data.get();
    }

    std::size_t font_image::getSize() const {
        auto res = reinterpret_cast<font_resources_impl *> (_resources.get());

        return res->_dataSize;
    }

    pixel_format font_image::getFormat() const {
        return pixel_format::RED;
    }

    void font_image::setSubimage(int, int, unsigned int, unsigned int, const image&) {
        __builtin_trap();
    }

    std::unique_ptr<char_sprite[]> font_image::encode(float x, float y, const std::string& text) const {
        auto res = reinterpret_cast<font_resources_impl *> (_resources.get());
        auto len = text.length();
        auto out = std::make_unique<char_sprite[]> (len);

        for (int i = 0; i < len; i++) {
            auto c = text[i];
            stbtt_aligned_quad q;

            stbtt_GetBakedQuad(res->_cdata.get(), res->_width, res->_height, (c - _info.firstChar), &x, &y, &q, true);

            out[i].texCoord.s0 = q.s0;
            out[i].texCoord.s1 = q.s1;
            out[i].texCoord.t0 = q.t0;
            out[i].texCoord.t1 = q.t1;
            out[i].vertex.x0 = q.x0;
            out[i].vertex.y0 = q.y0;
            out[i].vertex.x1 = q.x1;
            out[i].vertex.y1 = q.y1;
        }

        return out;
    }
}