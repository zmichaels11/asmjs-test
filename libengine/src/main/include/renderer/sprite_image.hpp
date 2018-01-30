#pragma once

#include "graphics/image.hpp"

#include "renderer/color_transform.hpp"

namespace renderer {
    struct sprite_image {
        graphics::image * pImage;        
        renderer::color_transform colorTransform;
    };
}