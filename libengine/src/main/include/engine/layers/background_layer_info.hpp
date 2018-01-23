#pragma once

#include "graphics/image.hpp"
#include "engine/layers/image_scroll_type.hpp"

namespace engine {
    namespace layers {
        struct background_layer_info {
            graphics::image * pImage;
            
            struct scroll_t {
                image_scroll_type horizontal, vertical;
            } scroll;
        };
    }
}