#pragma once

#include "engine/layers/hsv.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"

namespace engine {
    namespace layers {
        struct hsv_sprite_slot {
            parallelogram<float> shape;
            image_view view;
            hsv hsv;
            float padding;
        };
    }
}