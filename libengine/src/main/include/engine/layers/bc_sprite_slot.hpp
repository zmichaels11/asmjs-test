#pragma once

#include "engine/layers/bc.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/parallelogram.hpp"

namespace engine {
    namespace layers {
        struct bc_sprite_slot {
            engine::layers::parallelogram<float> shape;
            engine::layers::image_view view;
            engine::layers::bc bc;
            float padding[2];
        };
    }
}