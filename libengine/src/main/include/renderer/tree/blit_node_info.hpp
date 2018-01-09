#pragma once

#include "renderer/tree/base_node_info.hpp"
#include "renderer/tree/node_type.hpp"

namespace renderer {
    namespace tree {
        struct blit_node_info {
            renderer::tree::base_node_info baseInfo;
            renderer::tree::node_type childType;
            void * pChildInfo;
            void * pEffect;
        };
    }
}