#pragma once

#include <cstddef>

#include "graphics/pixel_type.hpp"
#include "graphics/pixel_format.hpp"

namespace graphics {
    struct pixel_info {
        pixel_type type;
        pixel_format format;
        const void * pData;
    };
}