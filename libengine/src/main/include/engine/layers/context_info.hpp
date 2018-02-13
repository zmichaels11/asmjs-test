#pragma once

#include <cstddef>

#include "graphics/image.hpp"

#include "engine/layers/renderable_info.hpp"
#include "engine/layers/sprite_sheet_info.hpp"
#include "engine/layers/sound_info.hpp"

namespace engine {
    namespace layers {
        struct context_info {
            sprite_sheet_info * pSpriteInfos;
            std::size_t nSpriteInfos;
            graphics::font_info * pFontInfos;
            std::size_t nFontInfos;
            renderable_info * pRenderableInfos;
            std::size_t nRenderableInfos;
            sound_info * pSoundResources;
            std::size_t nSoundResources;
        };
    }
}