#pragma once

#include "renderer/rgba.hpp"
#include "renderer/scene_layer_info.hpp"

namespace renderer {    
    struct scene_info {
        renderer::scene_layer_info* pLayerInfos;
        std::size_t layerInfoCount;

        struct clear_t {
            renderer::rgba color;
            float depth;
        } clear;
    };
}