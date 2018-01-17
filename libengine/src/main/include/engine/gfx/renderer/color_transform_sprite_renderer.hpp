#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class bc_sprite_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                color_transform_sprite_renderer(const color_transform_sprite_renderer&) = delete;

                color_transform_sprite_renderer& operator=(const color_transform_sprite_renderer&) = delete;

            public:
                color_transform_sprite_renderer(color_transform_sprite_renderer&&) = default;

                color_transform_sprite_renderer& operator=(color_transform_sprite_renderer&&) = default;

                color_transform_sprite_renderer(const color_transform_sprite_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_image_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}