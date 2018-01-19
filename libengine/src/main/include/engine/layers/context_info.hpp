#pragma once

#include <cstddef>

#include "graphics/image.hpp"

#include "engine/layers/sprite_sheet_info.hpp"

namespace engine {
    namespace layers {
        struct context_info {
            sprite_sheet_info * pSpriteInfos;
            std::size_t nSpriteInfos;
            graphics::font_info * pFontInfos;
            std::size_t nFontInfos;
        };
    }
}