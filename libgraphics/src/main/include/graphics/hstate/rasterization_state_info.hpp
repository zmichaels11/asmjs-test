#pragma once

#include "graphics/henum/cull_mode.hpp"
#include "graphics/henum/front_face.hpp"
#include "graphics/henum/polygon_mode.hpp"
#include "graphics/hstate/state.hpp"

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

        static rasterization_state_info defaults() noexcept;
    };

    template<>
    void apply (const graphics::rasterization_state_info& info) noexcept;
}