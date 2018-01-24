#pragma once

#include "engine/layers/renderable_image_info.hpp"
#include "engine/layers/renderable_type.hpp"
#include "engine/layers/tiled_image_info.hpp"

namespace engine {
    namespace layers {
        struct renderable_info {
            renderable_type type;
            union info_u {
                renderable_image_info imageInfo;
                tiled_image_info tiledImageInfo;
            } info;
        };
    }
}