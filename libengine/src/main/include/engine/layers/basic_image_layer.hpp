#pragma once

#include <memory>

#include "engine/layers/base_image_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/basic_image_layer_info.hpp"
#include "engine/layers/context.hpp"

namespace engine {
    namespace layers {
        class basic_image_layer : public base_image_layer {
            std::unique_ptr<base_resources> _pResources;

            basic_image_layer(const basic_image_layer&) = delete;

            basic_image_layer& operator=(const basic_image_layer&) = delete;

        public:
            basic_image_layer(basic_image_layer&&) = default;

            basic_image_layer& operator=(basic_image_layer&&) = default;

            basic_image_layer(
                const context& ctx,
                const basic_image_layer_info& info) noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void invalidate() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            virtual void scroll(float s0, float t0, float s1, float t1) noexcept;

            const basic_image_layer_info& getInfo() const noexcept;
        };
    }
}