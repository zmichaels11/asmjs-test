#pragma once

#include "renderer/layer_type.hpp"
#include "renderer/render_info.hpp"

namespace renderer {
    struct scene_layer_info {
        renderer::layer_type type;
        renderer::render_info renderInfo;
        void * pInfo;
    };
}