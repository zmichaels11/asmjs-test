#include "engine/gfx/graphics.hpp"

#include <cstring>

#include <memory>
#include <utility>

#include "engine/gfx/cmds/clear_clip_cmd.hpp"
#include "engine/gfx/cmds/clear_color_cmd.hpp"
#include "engine/gfx/cmds/clear_depth_cmd.hpp"
#include "engine/gfx/cmds/clip_rect_cmd.hpp"
#include "engine/gfx/cmds/draw_image_bc_cmd.hpp"
#include "engine/gfx/cmds/draw_image_cmd.hpp"
#include "engine/gfx/cmds/draw_image_color_transform_cmd.hpp"
#include "engine/gfx/cmds/draw_image_hsv_cmd.hpp"
#include "engine/gfx/cmds/draw_image_tint_cmd.hpp"
#include "engine/gfx/cmds/draw_line_cmd.hpp"
#include "engine/gfx/cmds/draw_rectangle_cmd.hpp"
#include "engine/gfx/cmds/draw_string_cmd.hpp"
#include "engine/gfx/cmds/draw_tile_cmd.hpp"
#include "engine/gfx/cmds/pop_matrix_cmd.hpp"
#include "engine/gfx/cmds/push_matrix_cmd.hpp"
#include "engine/gfx/cmds/rotate_transform_cmd.hpp"
#include "engine/gfx/cmds/scale_transform_cmd.hpp"
#include "engine/gfx/cmds/set_composite_mode_cmd.hpp"
#include "engine/gfx/cmds/set_transform_cmd.hpp"
#include "engine/gfx/cmds/translate_transform_cmd.hpp"

namespace engine {
    namespace gfx {
        void graphics::clearClip(const engine::gfx::rect<int, unsigned int>& clip) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::clear_clip_cmd> (clip);

            _commands.push(std::move(cmd));
        }

        void graphics::clearColor(const engine::gfx::color& color) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::clear_color_cmd> (color);

            _commands.push(std::move(cmd));
        }

        void graphics::clearDepth(float depth) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::clear_depth_cmd> (depth);

            _commands.push(std::move(cmd));
        }

        void graphics::setClip(
            const engine::gfx::rect<int, unsigned int>& clip, 
            engine::gfx::combine_mode combine) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::clip_rect_cmd> (combine, clip);

            _commands.push(std::move(cmd));
        }

        void graphics::drawTile(
            const engine::gfx::image * pImage,
            int x, int y) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_tile_cmd> (pImage, x, y);

            _commands.push(std::move(cmd));
        }
        
        void graphics::drawImageBC(
            const engine::gfx::image * pImage,
            const engine::gfx::parallelogram<float>& polygon,
            float brightness, float contrast) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_image_bc_cmd> (pImage, polygon, brightness, contrast);

            _commands.push(std::move(cmd));
        }

        void graphics::drawImage(
            const engine::gfx::image * pImage,
            const engine::gfx::parallelogram<float>& polygon) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_image_cmd> (pImage, polygon);

            _commands.push(std::move(cmd));
        }

        void graphics::drawImageColorTransform(
            const engine::gfx::image * pImage,
            const engine::gfx::parallelogram<float>& polygon,
            const engine::gfx::color_transform& colorTransform) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_image_color_transform_cmd>(pImage, polygon, colorTransform);

            _commands.push(std::move(cmd));
        }

        void graphics::drawImageHSV(
            const engine::gfx::image * pImage,
            const engine::gfx::parallelogram<float>& polygon,
            float hue, float saturation, float value) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_image_hsv_cmd> (pImage, polygon, hue, saturation, value);

            _commands.push(std::move(cmd));
        }

        void graphics::drawImageTint(
            const engine::gfx::image * pImage,
            const engine::gfx::parallelogram<float>& polygon,
            float tr, float tg, float tb, float ta) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_image_tint_cmd> (pImage, polygon, tr, tg, tb, ta);

            _commands.push(std::move(cmd));
        }

        void graphics::drawLine(
            const engine::gfx::pen& pen,
            const engine::gfx::point<float>& start,
            const engine::gfx::point<float>& end) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_line_cmd> (pen, start, end);

            _commands.push(std::move(cmd));
        }

        void graphics::drawRectangle(
            const engine::gfx::pen& pen,
            const engine::gfx::rect<float, float>& area) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_rectangle_cmd> (pen, area);

            _commands.push(std::move(cmd));
        }
            
        void graphics::drawString(
            const engine::gfx::font * pFont,
            const engine::gfx::pen& pen,
            const engine::gfx::point<float>& origin,
            const std::string& text) noexcept {

            auto cmd = std::make_unique<engine::gfx::cmds::draw_string_cmd> (pFont, pen, origin, text);

            _commands.push(std::move(cmd));
        }

        void graphics::setTransform(const float * matrix) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::set_transform_cmd> (matrix);

            _commands.push(std::move(cmd));
        }

        void graphics::rotate(float angle, engine::gfx::matrix_order order) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::rotate_transform_cmd> (angle, order);

            _commands.push(std::move(cmd));
        }

        void graphics::scale(float sx, float sy, engine::gfx::matrix_order order) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::scale_transform_cmd> (sx, sy, order);
        }

        void graphics::translate(float x, float y, engine::gfx::matrix_order order) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::translate_transform_cmd> (x, y, order);

            _commands.push(std::move(cmd));
        }

        void graphics::pushMatrix() noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::push_matrix_cmd> ();

            _commands.push(std::move(cmd));
        }

        void graphics::popMatrix() noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::pop_matrix_cmd> ();

            _commands.push(std::move(cmd));
        }

        void graphics::setCompositeMode(engine::gfx::composite_mode mode) noexcept {
            auto cmd = std::make_unique<engine::gfx::cmds::set_composite_mode_cmd> (mode);

            _commands.push(std::move(cmd));
        }
    }
}