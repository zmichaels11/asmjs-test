#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class bc_sprite_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                bc_sprite_renderer(const basic_sprite_renderer&) = delete;

                bc_sprite_renderer& operator=(const bc_sprite_renderer&) = delete;

            public:
                bc_sprite_renderer(bc_sprite_renderer&&) = default;

                bc_sprite_renderer& operator=(bc_sprite_renderer&&) = default;

                bc_sprite_renderer(const bc_sprite_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_image_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}