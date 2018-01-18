#pragma once

#include <cstddef>

#include "engine/layers/sprite_sheet_info.hpp"

namespace engine {
    namespace layers {
        struct context_info {
            engine::layers::sprite_sheet_info * pSpriteInfos;
            std::size_t nSpriteInfos;
        };
    }
}