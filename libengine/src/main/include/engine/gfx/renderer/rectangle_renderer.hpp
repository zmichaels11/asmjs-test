#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class rectangle_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                rectangle_renderer(const rectangle_renderer&) = delete;

                rectangle_renderer& operator=(const rectangle_renderer&) = delete;

            public:
                rectangle_renderer(rectangle_renderer&&) = default;

                rectangle_renderer& operator=(rectangle_renderer&&) = default;

                rectangle_renderer(const rectangle_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_rectangle_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}