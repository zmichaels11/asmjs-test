#pragma once

#include "graphics/henum/pixel_format.hpp"
#include "graphics/henum/pixel_type.hpp"

namespace graphics {
    enum class internal_format : unsigned int {
        UNKNOWN = 0,
        // ---- normalized, unsigned ----
        R8 = 0x8229,        
        RG8 = 0x822B,
        RGB8 = 0x8051,
        RGBA8 = 0x8058,
        R16 = 0x8222A,
        RG16 = 0x822C,
        RGB16 = 0x8054,
        RGBA16 = 0x805B,

        // ---- normalized, signed ----
        R8_SNORM = 0x8F94,
        RG8_SNORM = 0x8F95,
        RGB8_SNORM = 0x8F96,
        RGBA8_SNORM = 0xF97,
        R16_SNORM = 0xF98,
        RG16_SNORM = 0xF99,
        RGB16_SNORM = 0xF9A,
        RGBA16_SNORM = 0xF9B,

        // ---- floating point ----
        R16F = 0x822D,
        RG16F = 0x822F,
        RGB16F = 0x881B,
        RGBA16F = 0x881A,
        R32F = 0x822E,
        RG32F = 0x8230,
        RGB32F = 0x8815,
        RGBA32F = 0x8814,

        // ---- integer, unsigned ----
        R8UI = 0x8232,
        RG8UI = 0x8238,
        RGB8UI = 0x8D7D,
        RGBA8UI = 0x8D7C,
        R16UI = 0x8234,
        RG16UI = 0x823A,
        RGB16UI = 0x8D77,
        RGBA16UI = 0x8D76,
        R32UI = 0x8236,
        RG32UI = 0x823C,
        RGB32UI = 0x8D71,
        RGBA32UI = 0x8D70,

        // ---- integer, signed ----
        R8I = 0x8231,
        RG8I = 0x8237,
        RGB8I = 0x8D8F,
        RGBA8I = 0x8D8E,
        R16I = 0x8233,
        RG16I = 0x8239,
        RGB16I = 0x8d89,
        RGBA16I = 0x8D88,
        R32I = 0x8235,
        RG32I = 0x823B,
        RGB32I = 0x8D83,
        RGBA32I = 0x8D82,

        // ---- packed ----
        RGB10_A2 = 0x8059,
        RGBA4 = 0x8056,
        RGB565 = 0x8D62,
        RGB5_A1 = 0x8057,

        // ---- depth-stencil ----              
        DEPTH_COMPONENT16 = 0x81A5,
        DEPTH_COMPONENT24 = 0x81A6,
        DEPTH_COMPONENT32F = 0x8CAC,
        DEPTH24_STENCIL8 = 0x88F0,
        DEPTH32F_STENCIL8 = 0x8CAD,
        STENCIL_INDEX8 = 0x8D48
    };

    internal_format join(pixel_format fmt, pixel_type type);    

    internal_format join(pixel_format fmt, pixel_type type);

    inline internal_format join(pixel_type type, pixel_format fmt) {
        return join(fmt, type);
    }
}