#pragma once

#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"
#include "engine/layers/tint.hpp"

namespace engine {
    namespace layers {
        struct tint_sprite_slot {
            engine::layers::parallelogram<float> shape;
            engine::layers::image_view view;
            engine::layers::tint tint;
        };
    }
}