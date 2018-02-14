#pragma once

#include "graphics/hstate/state.hpp"

namespace graphics {
    struct viewport_state_info {
        int x, y;
        unsigned int width, height;
        float minDepth, maxDepth;
    };

    template<>
    void apply(const graphics::viewport_state_info& info) noexcept;
}