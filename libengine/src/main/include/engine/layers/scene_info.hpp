#pragma once

#include <cstddef>

#include "engine/layers/context_info.hpp"
#include "engine/layers/scene_layer_info.hpp"

namespace engine {
    namespace layers {
        struct scene_info {
            context_info contextInfo;
            scene_layer_info * pLayerInfos;
            std::size_t nLayerInfos;
            
            struct surface_size_t {
                unsigned int width;
                unsigned int height;
            } surface;
        };
    }
}