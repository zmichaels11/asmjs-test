#pragma once

#include <memory>
#include <string>

#include "math/mat4.hpp"

#include "engine/layers/base_layer.hpp"
#include "engine/layers/base_resources.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/tile_layer_info.hpp"
#include "engine/layers/tile_slot.hpp"

namespace engine {
    namespace layers {
        class tile_layer : public engine::layers::base_layer {
            std::unique_ptr<engine::layers::base_resources> _pResources;

            tile_layer(const tile_layer&) = delete;

            tile_layer& operator=(const tile_layer&) = delete;
        public:
            tile_layer(tile_layer&&) = default;

            tile_layer& operator=(tile_layer&&) = default;

            tile_layer(const tile_layer_info& info) noexcept;

            virtual void invalidate() noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void setProjection(const math::mat4& projection) noexcept;

            virtual void setProjection(const float * projection) noexcept;

            engine::layers::tile_slot ** fetchTileSlots() const noexcept;

            const engine::layers::image_view& getImageView(const std::string& imgRef) const noexcept;

            const engine::layers::tile_layer_info& getInfo() const noexcept;
        };
    }
}