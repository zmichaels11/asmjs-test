#pragma once

#include "engine/layers/tile_layer_hint.hpp"

namespace engine {
    namespace layers {
        struct tile_layer_info {
            unsigned int rows, columns;
            float tileSize;
            tile_layer_hint hints;
            int tileSheetID;
        };
    }
}