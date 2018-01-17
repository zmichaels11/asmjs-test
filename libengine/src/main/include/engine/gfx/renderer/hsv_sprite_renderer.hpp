#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class bc_sprite_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                hsv_sprite_renderer(const hsv_sprite_renderer&) = delete;

                hsv_sprite_renderer& operator=(const hsv_sprite_renderer&) = delete;

            public:
                hsv_sprite_renderer(hsv_sprite_renderer&&) = default;

                hsv_sprite_renderer& operator=(hsv_sprite_renderer&&) = default;

                hsv_sprite_renderer(const hsv_sprite_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_image_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}