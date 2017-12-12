#pragma once

#include "renderer/image_format.hpp"

namespace renderer {
    struct image {
        unsigned int width;
        unsigned int height;
        image_format format;
        void * data;
    };
}