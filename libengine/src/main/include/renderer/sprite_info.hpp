#pragma once

#include <cstdint>

#include "renderer/image_view.hpp"

namespace renderer {
    struct sprite_info {        
        struct position_t {
            float x, y;
        } position;
        
        struct offset_t {
            float x, y;
        } offset;
        
        struct scale_t {
            float x, y; 
        } scale;
        
        struct skew_t {
            float x, y;
        } skew;

        //sizeof(rotation) = 4
        float rotation;

        // sizeof(image_view) == 12
        void * spriteView;
        void * maskView;
        
        // sizeof(colorTransformId) = 4
        int colorTransformId;
    };
}