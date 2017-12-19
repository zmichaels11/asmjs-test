#pragma once

namespace renderer {
    struct scissor_rect {
        bool enabled;
        float x, y;
        float width, height;
    };        
}