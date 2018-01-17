#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class tile_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                tile_renderer(const tile_renderer&) = delete;

                tile_renderer& operator=(const tile_renderer&) = delete;

            public:
                tile_renderer(tile_renderer&&) = default;

                tile_renderer& operator=(tile_renderer&&) = default;

                tile_renderer(const tile_renderer& info);                

                void accept(const engine::gfx::cmds::draw_tile_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}