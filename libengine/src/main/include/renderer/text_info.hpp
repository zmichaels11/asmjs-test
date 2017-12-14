#pragma once

#include <string>

#include "renderer/rgba.hpp"

namespace renderer {
    struct text_info {
        std::string text;
        renderer::rgba color;
        float x, y;
    }; 
}