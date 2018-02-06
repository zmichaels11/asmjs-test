#pragma once

#include "graphics/henum/vertex_input_rate.hpp"

namespace graphics {
    class buffer;

    struct vertex_binding_description {
        unsigned int binding;
        unsigned int stride;        
        vertex_input_rate inputRate;
        
        buffer * pBuffer;
        long offset;
    };
}