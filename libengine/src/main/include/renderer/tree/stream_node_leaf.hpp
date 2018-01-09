#pragma once

namespace renderer {
    namespace tree {
        struct stream_node_leaf {
            struct position_t {
                float x, y;
            } position;

            int spriteId;

            float rotation;
        };
    }
}