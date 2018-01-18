#pragma once

namespace engine {
    namespace layers {
        template <typename point_component_T = float>
        struct point {
            point_component_T x, y;
        };
    }
}