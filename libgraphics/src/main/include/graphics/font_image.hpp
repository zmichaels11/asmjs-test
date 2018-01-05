#pragma once

#include <cstddef>

#include <memory>
#include <string>
#include <vector>

#include "graphics/font_info.hpp"
#include "graphics/image.hpp"
#include "graphics/pixel_format.hpp"

namespace graphics {
    struct font_resources {        
        virtual ~font_resources() noexcept {}
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
        std::shared_ptr<font_resources> _resources;

        font_image(const font_image&) = delete;

        font_image& operator=(const font_image&) = delete;
    public:
        font_image(font_image&&) = default;

        font_image& operator=(font_image&&) = default;

        font_image() noexcept:
            _info(),
            _resources(nullptr) {}

        font_image(const font_info& info) noexcept;

        virtual std::vector<char_sprite> encode(float x, float y, const std::string& text) const noexcept;

        virtual unsigned int getWidth() const noexcept;

        virtual unsigned int getHeight() const noexcept;

        virtual const void * getData() const noexcept;

        virtual std::size_t getSize() const noexcept;

        virtual pixel_format getFormat() const noexcept;

        virtual void setSubimage(int x, int y, unsigned int w, unsigned int h, const image& subimg) noexcept;

        float getAscent() const noexcept;

        float getDescent() const noexcept;

        float getLineGap() const noexcept;

        float getLineSpacing() const noexcept;
    };
}