#pragma once

#include <memory>

#include "engine/gfx/base_renderer.hpp"
#include "engine/gfx/base_resources.hpp"
#include "engine/gfx/quad_renderer_info.hpp"

namespace engine {
    namespace gfx {
        class quad_renderer {
            std::unique_ptr<base_resources> _pResources;

            quad_renderer(const quad_renderer&) = delete;

            quad_renderer& operator=(const quad_renderer&) = delete;

        public:
            quad_renderer() : 
                _pResources(nullptr) {}

            quad_renderer(quad_renderer&&) = default;

            quad_renderer& operator=(quad_renderer&&) = default;

            quad_renderer(const quad_renderer_info& info) noexcept;

            virtual void reset() noexcept;

            virtual void pushData(const void * pData) noexcept;

            virtual void update() noexcept;

            virtual void render() const noexcept;

            struct quad_data {
                float x, y, w, h;
                float u0, v0, u1, v1;
            };
        };
    }
}