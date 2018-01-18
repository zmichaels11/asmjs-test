#pragma once

#include <cstddef>

#include "engine/layers/write_hint.hpp"

namespace engine {
    namespace layers {
        struct tint_sprite_layer_info {
            std::size_t maxSprites;
            write_hint writeHint;
            int spriteID;
        };
    }
}