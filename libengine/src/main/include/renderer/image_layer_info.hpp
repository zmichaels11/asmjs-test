#pragma once

#include "renderer/color_transform.hpp"
#include "renderer/image.hpp"
#include "renderer/image_filter.hpp"
#include "renderer/image_scroll_type.hpp"

namespace renderer {
    struct image_layer_info {
        struct sublayer_info_t {
            renderer::image image;
            renderer::image_filter minFilter, magFilter;
            renderer::image_scroll_type hScroll, vScroll;
            renderer::color_transform* pColorTransform;
        } image, mask;
        
        bool supportsColorTransform;
    };
}