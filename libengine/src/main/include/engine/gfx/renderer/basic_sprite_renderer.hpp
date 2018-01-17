#pragma once

#include <memory>

#include "engine/gfx/cmds/draw_image_cmd.hpp"

#include "engine/gfx/renderer/base_renderer.hpp"
#include "engine/gfx/renderer/basic_sprite_renderer.info"

namespace engine {
    namespace gfx {
        namespace renderer {
            class basic_sprite_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                basic_sprite_renderer(const basic_sprite_renderer&) = delete;

                basic_sprite_renderer& operator=(const basic_sprite_renderer&) = delete;

            public:
                basic_sprite_renderer(basic_sprite_renderer&&) = default;

                basic_sprite_renderer& operator=(basic_sprite_renderer&&) = default;

                basic_sprite_renderer(const basic_sprite_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_image_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}