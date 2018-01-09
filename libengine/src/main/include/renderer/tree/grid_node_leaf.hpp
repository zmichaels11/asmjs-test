#pragma once

namespace renderer {
    namespace tree {
        struct grid_node_leaf {
            struct position_t {
                unsigned int x, y;
            } position;

            int spriteId;
        };
    }
}