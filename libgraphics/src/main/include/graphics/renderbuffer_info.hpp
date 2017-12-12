#pragma once

#include "graphics/internal_format.hpp"

namespace graphics {
    struct renderbuffer_info {
        graphics::internal_format format;
        unsigned int width;
        unsigned int height;        
    };
}