#pragma once

#include "graphics/image.hpp"
#include "engine/layers/sprite_sheet_hint.hpp"

namespace engine {
    namespace layers {
        struct sprite_sheet_info {
            graphics::image ** ppImages;
            std::size_t imageCount;
            sprite_sheet_hint hints;
        };
    }
}