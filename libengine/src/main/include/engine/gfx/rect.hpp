#pragma once

namespace engine {
    namespace gfx {
        template <typename position_T, typename size_T>
        struct rect {
            position_T x, y;
            size_T width, height;
        };
    }
}