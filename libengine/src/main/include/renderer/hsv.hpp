#pragma once

namespace renderer {
    struct hsv {
        float hue, saturation, value;        
    };

    inline void identity(hsv& it) {
        it.hue = 0.0F;
        it.saturation = 1.0F;
        it.value = 1.0F;
    }    
}