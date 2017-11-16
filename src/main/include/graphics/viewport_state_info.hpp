#pragma once

namespace graphics {
    struct viewport_state_info {
        int x, y, width, height;
        float minDepth, maxDepth;
    };

    void apply(const viewport_state_info& info);
}