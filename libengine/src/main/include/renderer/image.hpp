#pragma once

#include <memory>
#include <string>

#include "renderer/image_format.hpp"

namespace renderer {
    struct image {
        unsigned int width, height;
        renderer::image_format format;
        const void * pData;
    };    
}