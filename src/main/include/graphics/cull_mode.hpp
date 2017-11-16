#pragma once

namespace graphics {
    enum class cull_mode : unsigned int {
        NONE = 0,
        FRONT = 0x0404,
        BACK = 0x0405,
        FRONT_AND_BACK = 0x0408
    };
}