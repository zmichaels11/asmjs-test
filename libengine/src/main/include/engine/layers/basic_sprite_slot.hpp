#pragma once

#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"

namespace engine {
    namespace layers {
        struct basic_sprite_slot {
            engine::layers::parallelogram<float> shape;
            engine::layers::image_view view;
        };
    }
}