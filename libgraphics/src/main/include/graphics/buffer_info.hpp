#pragma once

#include <cstddef>

#include "graphics/buffer_target.hpp"
#include "graphics/buffer_usage.hpp"

namespace graphics {
    struct buffer_info {
        buffer_target target;
        buffer_usage usage;
        
        struct buffer_data_source_t {
            void * pData;
            std::size_t size;
        } initialData;
    };
}