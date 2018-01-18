#pragma once

#include "engine/layers/bc.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"

namespace engine {
    namespace layers {
        struct bc_sprite_slot {
            parallelogram<float> shape;
            image_view view;
            bc bc;
            float padding[2];
        };
    }
}