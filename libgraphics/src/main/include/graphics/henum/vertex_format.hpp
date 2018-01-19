#pragma once

#include <cstddef>

namespace graphics {
    enum class vertex_format : unsigned int {        
        X8Y8Z8W8_UNORM = 0,
        X8Y8Z8_UNORM,
        X8Y8_UNORM,
        X8_UNORM,
        
        X8Y8Z8W8_SNORM,
        X8Y8Z8_SNORM,
        X8Y8_SNORM,
        X8_SNORM,
                
        X16Y16Z16W16_UNORM,
        X16Y16Z16_UNORM,
        X16Y16_UNORM,
        X16_UNORM,

        X16Y16Z16W16_SNORM,
        X16Y16Z16_SNORM,
        X16Y16_SNORM,
        X16_SNORM,

        X32Y32Z32W32_SFLOAT,
        X32Y32Z32_SFLOAT,
        X32Y32_SFLOAT,
        X32_SFLOAT,

        X16Y16Z16W16_SFLOAT,
        X16Y16Z16_SFLOAT,
        X16Y16_SFLOAT,
        X16_SFLOAT,
        
        W2Z10Y10X10_UNORM,
        W2Z10Y10X10_SNORM,

        X8_UINT,
        X8_SINT,
        X8Y8_UINT,
        X8Y8_SINT,
        X8Y8Z8_UINT,
        X8Y8Z8_SINT,
        X8Y8Z8W8_UINT,
        X8Y8Z8W8_SINT,

        X16_UINT,
        X16_SINT,
        X16Y16_UINT,
        X16Y16_SINT,
        X16Y16Z16_UINT,
        X16Y16Z16_SINT,
        X16Y16Z16W16_UINT,
        X16Y16Z16W16_SINT,

        X32_UINT,
        X32_SINT,
        X32Y32_UINT,
        X32Y32_SINT,
        X32Y32Z32_UINT,
        X32Y32Z32_SINT,
        X32Y32Z32W32_UINT,
        X32Y32Z32W32_SINT,        
    };
}