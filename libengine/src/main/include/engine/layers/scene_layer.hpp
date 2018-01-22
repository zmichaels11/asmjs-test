#pragma once

#include <memory>

#include "engine/layers/base_layer.hpp"
#include "engine/layers/scene_layer_info.hpp"

namespace engine {
    namespace layers {
        struct scene_layer {
            scene_layer_info info;
            std::unique_ptr<base_layer> layer;            
        };
    }
}