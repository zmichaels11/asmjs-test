#pragma once

namespace engine {
    namespace layers {
        template <typename bounds_position_T = float, typename bounds_size_T = float>
        struct bounds {
            bounds_position_T x, y;
            bounds_size_T width, height;
        };
    }
}