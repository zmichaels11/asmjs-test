#pragma once

#include "renderer/image_view.hpp"

namespace renderer {
    // sizeof(sprite_info) == 64
    struct sprite_info {
        // sizeof(position_t) == 8
        struct position_t {
            float x, y;
        } position;

        // sizeof(offset_t) == 8
        struct offset_t {
            float x, y;
        } offset;

        // sizeof(scale_t) == 8
        struct scale_t {
            float x, y; 
        } scale;

        // sizeof(skew_t) == 8
        struct skew_t {
            float x, y;
        } skew;

        //sizeof(rotation) = 4
        float rotation;

        // sizeof(image_view) == 12
        renderer::image_view spriteView;
        renderer::image_view maskView;
        
        // sizeof(colorTransformId) = 4
        int colorTransformId;
    };
}