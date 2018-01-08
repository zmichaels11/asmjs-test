#include "graphics/hstate/rasterization_state_info.hpp"

namespace graphics {    
    rasterization_state_info rasterization_state_info::defaults() noexcept {
        return {
            front_face::CCW,
            1.0F,
            cull_mode::BACK,
            {false, 0.0F, 0.0F},
            polygon_mode::FILL
        };
    }
}