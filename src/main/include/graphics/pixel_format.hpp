#pragma once

namespace graphics {
    enum class pixel_format : unsigned int {
        RGBA = 0x1908,
        RGB = 0x1907,
        RG = 0x8227, 
        RED = 0x1903,
        RGBA_INTEGER = 0x8D99,
        RGB_INTEGER = 0x8D98,
        RG_INTEGER = 0x8228,
        RED_INTEGER = 0x8D94
    };
}