#pragma once

#include <memory>

#include "engine/gfx/base_renderer.hpp"
#include "engine/gfx/base_resources.hpp"
#include "engine/gfx/tile_renderer_info.hpp"

namespace engine {
    namespace gfx {        
        class tile_renderer : public virtual base_renderer {
            std::unique_ptr<base_resources> _pResources;
        
            tile_renderer(const tile_renderer&) = delete;

            tile_renderer& operator=(const tile_renderer&) = delete;

        public:
            tile_renderer(tile_renderer&&) = default;

            tile_renderer& operator=(tile_renderer&&) = default;

            tile_renderer(const tile_renderer_info& info) noexcept;

            virtual ~tile_renderer();

            virtual void pushData(const void * pData) noexcept;

            virtual void reset() noexcept;

            virtual void update() noexcept;

            virtual void render() const noexcept;
            
            enum class data_type {
                PROJECTION,
                TILE
            };

            struct tile_data {
                unsigned int x, y;
                int tileId;
            };

            struct data {
                data_type type;

                union payload_u {
                    tile_data tile;
                    float projection[16];
                } payload;
            };
        };
    }
}