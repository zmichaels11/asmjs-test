#pragma once

#include <memory>
#include <queue>

#include "engine/gfx/cmds/command.hpp"

#include "engine/gfx/combine_mode.hpp"
#include "engine/gfx/composite_mode.hpp"
#include "engine/gfx/color_transform.hpp"
#include "engine/gfx/color.hpp"
#include "engine/gfx/font.hpp"
#include "engine/gfx/image.hpp"
#include "engine/gfx/matrix_order.hpp"
#include "engine/gfx/parallelogram.hpp"
#include "engine/gfx/pen.hpp"
#include "engine/gfx/point.hpp"
#include "engine/gfx/rect.hpp"

namespace engine {
    namespace gfx {
        class graphics {            
            std::queue<std::unique_ptr<engine::gfx::cmds::command>> _commands;            
            unsigned int _tileSize;

        public:
            graphics(unsigned int tileSize = 32) :
                _commands(),
                _tileSize(tileSize){}

            void clearClip(const engine::gfx::rect<int, unsigned int>& rect) noexcept;

            void clearColor(const engine::gfx::color& color) noexcept;
            
            void clearDepth(float depth) noexcept;

            void setClip(
                const engine::gfx::rect<int, unsigned int>& clip, 
                engine::gfx::combine_mode combine = engine::gfx::combine_mode::REPLACE) noexcept;

            void drawTile(
                const engine::gfx::image * pImage,
                int x, int y) noexcept;
            
            void drawImageBC(
                const engine::gfx::image * pImage,
                const engine::gfx::parallelogram<float>& polygon,
                float brightness, float contrast) noexcept;

            void drawImage(
                const engine::gfx::image * pImage,
                const engine::gfx::parallelogram<float>& polygon) noexcept;

            void drawImageColorTransform(
                const engine::gfx::image * pImage,
                const engine::gfx::parallelogram<float>& polygon,
                const engine::gfx::color_transform& colorTransform) noexcept;

            void drawImageHSV(
                const engine::gfx::image * pImage,
                const engine::gfx::parallelogram<float>& polygon,
                float hue, float saturation, float value) noexcept;

            void drawImageTint(
                const engine::gfx::image * pImage,
                const engine::gfx::parallelogram<float>& polygon,
                float tr, float tg, float tb, float ta = 1.0F) noexcept;

            void drawLine(
                const engine::gfx::pen& pen,
                const engine::gfx::point<float>& start,
                const engine::gfx::point<float>& end) noexcept;

            void drawRectangle(
                const engine::gfx::pen& pen,
                const engine::gfx::rect<float, float>& area) noexcept;
                
            void drawString(
                const engine::gfx::font * pFont,
                const engine::gfx::pen& pen,
                const engine::gfx::point<float>& origin,
                const std::string& text) noexcept;

            void setTransform(const float * matrix) noexcept;

            void rotate(float angle, engine::gfx::matrix_order order = engine::gfx::matrix_order::PREPEND) noexcept;

            void scale(float sx, float sy, engine::gfx::matrix_order order = engine::gfx::matrix_order::PREPEND) noexcept;

            void translate(float x, float y, engine::gfx::matrix_order order = engine::gfx::matrix_order::PREPEND) noexcept;

            void pushMatrix() noexcept;

            void popMatrix() noexcept;

            void setCompositeMode(engine::gfx::composite_mode mode) noexcept;
        };
    }
}