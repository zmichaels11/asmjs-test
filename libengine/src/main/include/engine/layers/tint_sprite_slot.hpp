#pragma once

#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"
#include "engine/layers/tint.hpp"

namespace engine {
    namespace layers {
        struct tint_sprite_slot {
            parallelogram<float> shape;
            image_view view;
            engine::layers::tint tint;
        };
    }
}