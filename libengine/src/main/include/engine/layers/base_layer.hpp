#pragma once

#include "math/mat4.hpp"

namespace engine {
    namespace layers {
        class base_layer {
        public:
            virtual ~base_layer() {}

            virtual void beginWrite() noexcept = 0;

            virtual void endWrite() noexcept = 0;

            virtual void render() const noexcept = 0;

            virtual void invalidate() noexcept = 0;

            virtual void setProjection(const math::mat4& projection) noexcept = 0;

            virtual void setProjection(const float * projection) noexcept = 0;
        };
    }
}