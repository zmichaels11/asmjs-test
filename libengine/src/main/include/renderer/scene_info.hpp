#pragma once

#include <vector>

#include "renderer/rgba.hpp"
#include "renderer/scene_layer_info.hpp"

namespace renderer {    
    struct scene_info {
        std::vector<renderer::scene_layer_info> layerInfos;

        struct viewport_info_t {
            float x, y;
            float width, height;
            float near, far;
        } viewport;

        struct scissor_info_t {
            float x, y;
            float width, height;
        } scissor;

        struct clear_t {
            renderer::rgba color;
            float depth;
        } clear;
    };
}