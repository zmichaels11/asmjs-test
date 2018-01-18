#pragma once

#include "engine/layers/bounds.hpp"
#include "engine/layers/sprite_sheet_info.hpp"
#include "engine/layers/tile_layer_hint.hpp"

namespace engine {
    namespace layers {
        struct tile_layer_info {
            unsigned int rows, columns;
            float tileSize;
            engine::layers::tile_layer_hint hints;
            engine::layers::sprite_sheet_info tileInfo;
        };
    }
}