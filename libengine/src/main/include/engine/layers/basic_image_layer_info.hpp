#pragma once

#include "graphics/image.hpp"

#include "engine/layers/scroll_type.hpp"

namespace renderer {
    struct image_layer_info {
        graphics::image * pImage;
        struct scroll_t {
            scroll_type horizontal, vertical;
        } scroll;
    };
}