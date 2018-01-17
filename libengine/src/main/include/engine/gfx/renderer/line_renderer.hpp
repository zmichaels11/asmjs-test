#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class line_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                line_renderer(const line_renderer&) = delete;

                line_renderer& operator=(const line_renderer&) = delete;

            public:
                line_renderer(line_renderer&&) = default;

                line_renderer& operator=(line_renderer&&) = default;

                line_renderer(const line_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_line_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}