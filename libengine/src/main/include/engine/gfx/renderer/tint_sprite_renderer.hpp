#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class tint_sprite_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                tint_sprite_renderer(const tint_sprite_renderer&) = delete;

                tint_sprite_renderer& operator=(const tint_sprite_renderer&) = delete;

            public:
                tint_sprite_renderer(tint_sprite_renderer&&) = default;

                tint_sprite_renderer& operator=(tint_sprite_renderer&&) = default;

                tint_sprite_renderer(const tint_sprite_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_tint_image_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}