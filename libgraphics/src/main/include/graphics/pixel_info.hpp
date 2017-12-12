#pragma once

#include <cstddef>

#include "graphics/pixel_type.hpp"
#include "graphics/pixel_format.hpp"

namespace graphics {
    struct pixel_info {
        graphics::pixel_type type;
        graphics::pixel_format format;
        void * pData;
    };
}