#include "graphics/rasterization_state_info.hpp"

namespace graphics {
    template<>
    rasterization_state_info defaults() {
        return {
            front_face::CCW,
            1.0F,
            cull_mode::BACK,
            {false, 0.0F, 0.0F},
            polygon_mode::FILL
        };
    }
}