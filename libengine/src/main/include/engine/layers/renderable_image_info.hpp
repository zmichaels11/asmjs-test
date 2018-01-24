#pragma once

#include "graphics/image.hpp"

#include "engine/layers/image_filter_type.hpp"
#include "engine/layers/image_scroll_type.hpp"

namespace engine {
    namespace layers {
        struct renderable_image_info {
            struct scroll_t {
                image_scroll_type horizontal;
                image_scroll_type vertical;
            } scroll;

            image_filter_type filterType;
            graphics::image * initialImage;
        };
    }
}