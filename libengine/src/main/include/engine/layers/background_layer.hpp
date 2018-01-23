#pragma once

#include <memory>

#include "math/mat2.hpp"
#include "math/mat4.hpp"

#include "engine/layers/background_layer_info.hpp"
#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"

namespace engine {
    namespace layers {
        class background_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;

            background_layer(const background_layer&) = delete;

            background_layer& operator= (const background_layer&) = delete;
            
        public:
            background_layer(background_layer&&) = default;

            background_layer& operator= (background_layer&&) = default;

            background_layer(const context& ctx, const background_layer_info& info) noexcept;                                

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void invalidate() noexcept;

            virtual void render() const noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            const background_layer_info& getInfo() const noexcept;

            void scroll(float h, float v) noexcept;

            void setOrigin(float x, float y) noexcept;

            void setTransform(const float * transform) noexcept;

            void setTransform(const math::mat2& transform) noexcept;

            void setTransform(float a, float b, float c, float d) noexcept;
        };
    }
}