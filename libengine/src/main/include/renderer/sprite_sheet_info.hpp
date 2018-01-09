#pragma once

#include <cstddef>

#include "renderer/sprite_image.hpp"

namespace renderer {
    

    struct sprite_sheet_info {
        const renderer::sprite_image * pSpriteImages;        
        std::size_t spriteImageCount;
    };
}