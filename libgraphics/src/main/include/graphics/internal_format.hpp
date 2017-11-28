#pragma once

namespace graphics {
    enum class internal_format : unsigned int {
        R8 = 0x8229,        
        RG8 = 0x822B,
        RGB8 = 0x8051,
        RGBA8 = 0x8058,
        R16F = 0x822D,
        RG16F = 0x822F,
        RGB16F = 0x881B,
        RGBA16F = 0x881A,
        RGB10_A2 = 0x8059,
        RGBA4 = 0x8056,
        RGB565 = 0x8D62,
        RGB5_A1 = 0x8057,                
        DEPTH_COMPONENT16 = 0x81A5,
        DEPTH_COMPONENT24 = 0x81A6,
        DEPTH_COMPONENT32F = 0x8CAC,
        DEPTH24_STENCIL8 = 0x88F0,
        DEPTH32F_STENCIL8 = 0x8CAD,
        STENCIL_INDEX8 = 0x8D48
    };
}