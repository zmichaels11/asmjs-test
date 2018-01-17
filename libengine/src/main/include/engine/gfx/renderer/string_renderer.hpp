#pragma once

#include <memory>

#include "engine/gfx/renderer/base_renderer.hpp"

namespace engine {
    namespace gfx {
        namespace renderer {
            class string_renderer : public engine::gfx::renderer::base_renderer {
                std::unique_ptr<base_renderer_resources> _pResources;

                rectangle_string_rendererrenderer(const string_renderer&) = delete;

                string_renderer& operator=(const string_renderer&) = delete;

            public:
                string_renderer(string_renderer&&) = default;

                string_renderer& operator=(string_renderer&&) = default;

                string_renderer(const string_renderer_info& info);                

                void accept(const engine::gfx::cmds::draw_string_cmd& cmd) noexcept;

                virtual void flush() noexcept;
            }
        }
    }
}