#pragma once

#include <cstddef>

#include <memory>
#include <string>

#include "graphics/font_info.hpp"
#include "graphics/image.hpp"
#include "graphics/pixel_format.hpp"

namespace graphics {
    struct font_resources {        
        virtual ~font_resources() {}
    };

    struct char_sprite {
        struct tex_quad_t {
            float s0, t0, s1, t1;
        } texCoord;

        struct vtx_quad_t {
            float x0, y0, x1, y1;
        } vertex;
    };

    class font_image : public image {
        font_info _info;
        std::unique_ptr<font_resources> _resources;

        font_image(const font_image&) = delete;

        font_image& operator=(const font_image&) = delete;
    public:
        font_image(const font_info& info);

        font_image(font_image&&) = default;

        font_image& operator=(font_image&&) = default;

        virtual std::unique_ptr<char_sprite[]> encode(float x, float y, const std::string& text) const;

        virtual unsigned int getWidth() const;

        virtual unsigned int getHeight() const;

        virtual const void * getData() const;

        virtual std::size_t getSize() const;

        virtual pixel_format getFormat() const;

        virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimg);
    };
}