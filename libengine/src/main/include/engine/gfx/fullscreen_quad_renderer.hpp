#pragma once

#include <memory>

#include "engine/gfx/base_renderer.hpp"
#include "engine/gfx/base_resources.hpp"

namespace engine {
    namespace gfx {
        class fullscreen_quad_renderer : public virtual base_renderer {            
            std::unique_ptr<base_resources> _pResources;

        public:
            fullscreen_quad_renderer() noexcept;

            virtual ~fullscreen_quad_renderer();

            virtual void pushData(const void * pData) noexcept;

            virtual void reset() noexcept;

            virtual void render() const noexcept;
        };
    }
}