#pragma once

#include "graphics/cull_mode.hpp"
#include "graphics/front_face.hpp"
#include "graphics/polygon_mode.hpp"

namespace graphics {
    struct rasterization_state_info {
        graphics::front_face frontFace;
        float lineWidth;
        graphics::cull_mode cullMode;
        
        struct polygon_offset_T {
            bool enabled;
            float factor;
            float units;
        } polygonOffset;

        graphics::polygon_mode polygonMode;
    };

    void apply (const graphics::rasterization_state_info& info);
}