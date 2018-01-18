#pragma once

#include <memory>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/context.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/tile_layer_info.hpp"
#include "engine/layers/tile_slot.hpp"

namespace engine {
    namespace layers {
        class tile_layer : public base_layer {
            std::unique_ptr<base_resources> _pResources;

            tile_layer(const tile_layer&) = delete;

            tile_layer& operator=(const tile_layer&) = delete;
        public:
            tile_layer(tile_layer&&) = default;

            tile_layer& operator=(tile_layer&&) = default;

            tile_layer(
                const context& ctx,
                const tile_layer_info& info) noexcept;

            virtual void invalidate() noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            tile_slot ** fetchTileSlots() const noexcept;

            const image_view& getImageView(int tileID) const noexcept;

            const tile_layer_info& getInfo() const noexcept;
        };
    }
}