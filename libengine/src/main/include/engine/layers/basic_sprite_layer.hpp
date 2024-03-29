#pragma once

#include <memory>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/basic_sprite_layer_info.hpp"
#include "engine/layers/basic_sprite_slot.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_view.hpp"

namespace engine {
    namespace layers {        
        class basic_sprite_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;

            basic_sprite_layer(const basic_sprite_layer&) = delete;

            basic_sprite_layer& operator=(const basic_sprite_layer&) = delete;

        public:
            basic_sprite_layer(basic_sprite_layer&&) = default;

            basic_sprite_layer& operator=(basic_sprite_layer&&) = default;

            basic_sprite_layer(
                const context& ctx, 
                const basic_sprite_layer_info& info) noexcept;

            virtual void invalidate() noexcept;      

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            basic_sprite_slot ** fetchSpriteSlots() const noexcept;

            const image_view& getImageView(int spriteID) const noexcept;

            const basic_sprite_layer_info& getInfo() const noexcept;
        };
    }
}