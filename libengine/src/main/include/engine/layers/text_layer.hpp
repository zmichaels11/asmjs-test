#pragma once

#include <cstddef>

#include <memory>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/text_layer_info.hpp"
#include "engine/layers/text_info.hpp"

namespace engine {
    namespace layers {
        class text_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;
        
            text_layer(const text_layer&) = delete;

            text_layer& operator=(const text_layer&) = delete;

        public:
            text_layer(text_layer&&) = default;

            text_layer& operator=(text_layer&&) = default;

            text_layer(
                const context& ctx, 
                const text_layer_info& info) noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void invalidate() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            void submit(const text_info * pTexts, std::size_t count = 1) noexcept;
        };
    }
}