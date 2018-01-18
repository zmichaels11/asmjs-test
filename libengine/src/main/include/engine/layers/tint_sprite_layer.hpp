#pragma once

#include <memory>
#include <string>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/tint_sprite_layer_info.hpp"
#include "engine/layers/tint_sprite_slot.hpp"

namespace engine {
    namespace layers {
        class tint_sprite_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;

            tint_sprite_layer(const tint_sprite_layer&) = delete;

            tint_sprite_layer& operator=(const tint_sprite_layer&) = delete;

        public:
            tint_sprite_layer(tint_sprite_layer&&) = default;
            
            tint_sprite_layer& operator=(tint_sprite_layer&&) = default;

            tint_sprite_layer(
                const context& ctx,
                const tint_sprite_layer_info& info) noexcept;

            virtual void invalidate() noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            engine::layers::tint_sprite_slot ** fetchSpriteSlots() const noexcept;

            const engine::layers::image_view& getImageView(int spriteID) const noexcept;

            const engine::layers::tint_sprite_layer_info& getInfo() const noexcept;
        };
    }
}