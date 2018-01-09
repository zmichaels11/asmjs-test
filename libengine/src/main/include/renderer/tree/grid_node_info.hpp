#pragma once

#include "renderer/tree/base_node_info.hpp"

namespace renderer {
    namespace tree {
        struct grid_node_info {
            renderer::tree::base_node_info baseInfo;
            unsigned int columns;
            unsigned int rows;
            unsigned int cellWidth;
            unsigned int cellHeight;            
        };
    }
}