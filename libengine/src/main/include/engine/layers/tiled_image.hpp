#pragma once

#include <memory>

#include "engine/layers/context.hpp"
#include "engine/layers/renderable.hpp"
#include "engine/layers/tiled_image_info.hpp"
#include "engine/layers/tile_slot.hpp"

namespace engine {
    namespace layers {
        class tiled_image : public renderable {
            std::unique_ptr<base_resources> _pResources;

            tiled_image(const tiled_image&) = delete;

            tiled_image& operator=(const tiled_image&) = delete;

        public:
            tiled_image(tiled_image&&) = default;

            tiled_image& operator=(tiled_image&&) = default;

            tiled_image(
                const context * pctx,
                const tiled_image_info& info) noexcept;

            virtual void beginWrite() noexcept;

            virtual void endWrite() noexcept;

            virtual void render() const noexcept;

            virtual void invalidate() noexcept;

            virtual const void * getTexture() const noexcept;

            tile_slot ** fetchTileSlots() noexcept;            

            float ** fetchOffsets() noexcept;

            const tiled_image_info& getInfo() const noexcept;            
        };
    }
}