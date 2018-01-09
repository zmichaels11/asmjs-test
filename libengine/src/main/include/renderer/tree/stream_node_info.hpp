#pragma once

#include "renderer/sprite_sheet_info.hpp"
#include "renderer/tree/base_node_info.hpp"

namespace renderer {
    namespace tree {
        struct stream_node_info {
            base_node_info baseInfo;
            unsigned int nLeaves;
            renderer::sprite_sheet_info * pSpritesheetInfo;
            bool hasColorTransform;            
        };
    }
}