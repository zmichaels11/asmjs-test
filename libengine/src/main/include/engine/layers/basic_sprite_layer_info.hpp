#pragma once

#include <cstddef>

#include "engine/layers/sprite_sheet_info.hpp"
#include "engine/layers/write_hint.hpp"

namespace engine {
    namespace layers {
        struct basic_sprite_layer_info {
            std::size_t maxSprites;                        
            write_hint writeHint;
            int spriteSheetID;
        };
    }
}