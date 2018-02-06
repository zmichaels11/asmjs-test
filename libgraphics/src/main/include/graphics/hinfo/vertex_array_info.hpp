#pragma once

#include <cstddef>

#include "graphics/hinfo/vertex_attribute_description.hpp"
#include "graphics/hinfo/vertex_binding_description.hpp"

namespace graphics {    
    class buffer;
    
    struct vertex_array_info {
        graphics::vertex_attribute_description * pAttributes;
        std::size_t nAttributes;
        graphics::vertex_binding_description * pBindings;
        std::size_t nBindings;
        graphics::buffer * pIndexBuffer;
    };
}