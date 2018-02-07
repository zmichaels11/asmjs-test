#include "pch.h"
#include "graphics/hobject/font_image.hpp"

#include <vector>

#include "graphics/hinfo/font_info.hpp"
#include "graphics/henum/pixel_format.hpp"
#include "graphics/henum/pixel_type.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;

        struct font_resources_impl : public virtual font_resources {
            std::unique_ptr<stbtt_bakedchar[]> _cdata;
            stbtt_fontinfo _fontInfo;
            float _scale;            
            float _ascent, _descent, _lineGap, _lineSpacing;
            std::unique_ptr<unsigned char[]> _data;
            std::size_t _dataSize;
            unsigned int _width;
            unsigned int _height;

            font_resources_impl(const font_info& info) noexcept;

            virtual ~font_resources_impl() noexcept {
            }
        };
    }

    float font_image::getLineSpacing() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_lineSpacing;
    }
    
    float font_image::getAscent() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_ascent; 
    }

    float font_image::getDescent() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_descent;
    }

    float font_image::getLineGap() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_lineGap;
    }

    font_image::font_image(const font_info& info) noexcept {
        _info = info;
        _resources.reset(new font_resources_impl(info));
    }

    unsigned int font_image::getWidth() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_width;
    }

    unsigned int font_image::getHeight() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_height;
    }

    const void * font_image::getData() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_data.get();
    }

    std::size_t font_image::getSize() const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());

        return res->_dataSize;
    }

    pixel_format font_image::getFormat() const noexcept {
        return pixel_format::RED;
    }

    pixel_type font_image::getType() const noexcept {
        return pixel_type::UNSIGNED_BYTE;
    }

    void font_image::setSubimage(int, int, unsigned int, unsigned int, const image&) noexcept {
        _onError("font_image does not support writing!");
    }

    std::vector<char_sprite> font_image::encode(float x, float y, const std::string& text) const noexcept {
        auto res = dynamic_cast<font_resources_impl *> (_resources.get());
        auto len = text.length();
        auto out = std::vector<char_sprite>();

        for (int i = 0; i < len; i++) {
            auto c = text[i];
            auto q = stbtt_aligned_quad{0};            
            auto codeIndex = c - _info.firstChar;

            stbtt_GetBakedQuad(res->_cdata.get(), res->_width, res->_height, codeIndex, &x, &y, &q, true);

            out.push_back({{q.s0, q.t0, q.s1, q.t1}, {q.x0, q.y0, q.x1, q.y1}});
        }

        return out;
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[gfx] font_image error: " << msg << std::endl;
            __builtin_trap();
        }

        font_resources_impl::font_resources_impl(const font_info& info) noexcept {
                auto fontData = util::readAll(info.fontFile);

                stbtt_InitFont(&_fontInfo, reinterpret_cast<const unsigned char *> (fontData.get()), 0);
                _scale = stbtt_ScaleForPixelHeight(&_fontInfo, info.charHeight);

                int ascent, descent, lineGap;
                stbtt_GetFontVMetrics(&_fontInfo, &ascent, &descent, &lineGap);

                _ascent = _scale * ascent;
                _descent = _scale * descent;
                _lineGap = _scale * lineGap;
                _lineSpacing = _ascent - _descent + _lineGap;
                _cdata = std::make_unique<stbtt_bakedchar[]> (info.charCount);

                auto neededArea = info.charCount * info.charHeight * info.charHeight;
                auto flipflop = false;
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
    }
}
