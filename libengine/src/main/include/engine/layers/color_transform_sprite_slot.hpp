#pragma once

#include "engine/layers/color_transform.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"

namespace engine {
    namespace layers {
        struct color_transform_sprite_slot {
            engine::layers::parallelogram<float> shape;
            engine::layers::image_view view;
            engine::layers::color_transform colorTransform;
        };
    }
}