#pragma once

#include "renderer/layer_type.hpp"

namespace renderer {
    struct scene_layer_info {
        renderer::layer_type type;
        void * pInfo;
    };
}