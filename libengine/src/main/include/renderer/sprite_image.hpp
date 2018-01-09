#pragma once

#include "renderer/color_transform.hpp"
#include "renderer/image.hpp"

namespace renderer {
    struct sprite_image {
        renderer::image image;        
        renderer::color_transform colorTransform;
    };
}