#pragma once

#include "renderer/color_transform.hpp"
#include "renderer/image.hpp"
#include "renderer/image_filter.hpp"
#include "renderer/image_scroll_type.hpp"

namespace renderer {
    struct image_layer_info {
        renderer::image image;
        struct filter_t {
            renderer::image_filter min;
            renderer::image_filter mag;
        } filter;     
        
        struct scroll_t {
            renderer::image_scroll_type horizontal;
            renderer::image_scroll_type vertical;
        } scroll;
    };
}