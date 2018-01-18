#pragma once

#include <string>

#include "engine/layers/color.hpp"

namespace engine {
    namespace layers {    
        struct text_info {
            std::string text;
            engine::layers::color color;
            float x, y;
        }; 
    }
}