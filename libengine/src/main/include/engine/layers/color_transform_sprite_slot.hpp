#pragma once

#include "engine/layers/color_transform.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"

namespace engine {
    namespace layers {
        struct color_transform_sprite_slot {
            parallelogram<float> shape;
            image_view view;
            color_transform colorTransform;
        };
    }
}