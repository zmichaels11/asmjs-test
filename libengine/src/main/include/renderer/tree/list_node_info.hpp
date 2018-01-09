#pragma once

#include <cstddef>

#include "renderer/tree/base_node_info.hpp"
#include "renderer/tree/node_type.hpp"

namespace renderer {
    namespace tree {
        struct list_node_info {
            renderer::tree::base_node_info baseInfo;
            std::size_t nChildren;
            void ** pChildInfos;
        };
    }
}