#pragma once

#include "renderer/tree/blend_mode.hpp"
#include "renderer/tree/bounds.hpp"

namespace renderer {
    namespace tree {
        struct base_node_info {
            renderer::tree::blend_mode blendMode;
            renderer::tree::bounds bounds;
            bool hasColorTransform;               
        };
    }
}