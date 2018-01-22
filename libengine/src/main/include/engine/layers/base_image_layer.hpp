#pragma once

#include "engine/layers/base_layer.hpp"

namespace engine {
    namespace layers {
        class base_image_layer : public base_layer {
        public:
            virtual ~base_image_layer() {}
            
            virtual void beginWrite() noexcept = 0;

            virtual void endWrite() noexcept = 0;

            virtual void render() const noexcept = 0;

            virtual void invalidate() noexcept = 0;

            virtual void setProjection(const math::mat4& projection) noexcept = 0;

            virtual void setProjection(const float * projection) noexcept = 0;

            virtual void scroll(float s0, float t0, float s1, float t1) noexcept = 0;
        };
    }
}