#pragma once

#include <vector>

#include "renderer/sprite_resource_info.hpp"

namespace renderer {
    struct sprite_layer_info {
        std::vector<renderer::sprite_resource_info> validImages;
        std::vector<renderer::image> validMasks;
        unsigned int maxSprites;
        unsigned int maxColorTransforms;
        bool streaming;
    };
}