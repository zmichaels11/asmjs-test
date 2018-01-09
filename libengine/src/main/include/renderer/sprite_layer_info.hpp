#pragma once

#include "renderer/sprite_sheet_info.hpp"
#include "renderer/sprite_resource_info.hpp"

namespace renderer {
    struct sprite_layer_info {
        const renderer::sprite_sheet_info * pSpritesheet;
        unsigned int maxSprites;
        unsigned int maxColorTransforms;
        bool streaming;
    };
}