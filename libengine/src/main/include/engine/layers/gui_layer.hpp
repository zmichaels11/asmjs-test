#pragma once

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"

namespace engine {
    namespace layers {
        class gui_layer : public base_layer {
            gui_layer(const gui_layer&) = delete;

            gui_layer& operator=(const gui_layer&) = delete;

        public:
            gui_layer() {}

            gui_layer(gui_layer&&) = default;
            
            gui_layer& operator=(gui_layer&&) = default;
            
            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void invalidate() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;
        };
    }
}