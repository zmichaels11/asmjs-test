#pragma once

#include <cstddef>

#include "engine/layers/sprite_sheet_info.hpp"
#include "engine/layers/write_hint.hpp"

namespace engine {
    namespace layers {
        struct tint_sprite_layer_info {
            std::size_t maxSprites;
            engine::layers::write_hint writeHint;
            engine::layers::sprite_sheet_info spriteInfo;
        };
    }
}