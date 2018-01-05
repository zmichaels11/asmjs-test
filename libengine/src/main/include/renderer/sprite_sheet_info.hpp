#pragma once

#include <cstddef>

#include "renderer/color_transform.hpp"
#include "renderer/image.hpp"

namespace renderer {
    struct sprite_image {
        renderer::image image;        
        renderer::color_transform colorTransform;
    };

    struct sprite_sheet_info {
        const sprite_image * pSpriteImages;        
        std::size_t spriteImageCount;
    };
}