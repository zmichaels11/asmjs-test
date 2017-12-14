#pragma once

namespace renderer {
    enum class layer_type : unsigned int {
        SPRITE = 0,
        IMAGE = 1,
        TEXT = 2,
        TILE = 3,
        GUI = 4
    };
}