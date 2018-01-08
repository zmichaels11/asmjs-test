#pragma once

#include <cstddef>

#include "graphics/hinfo/vertex_attribute_description.hpp"
#include "graphics/hinfo/vertex_binding_description.hpp"

namespace graphics {    
    class buffer;
    
    struct vertex_array_info {
        graphics::vertex_attribute_description ** ppAttributes;
        std::size_t attributeCount;
        graphics::vertex_binding_description ** ppBindings;
        std::size_t bindingCount;
        graphics::buffer * pIndexBuffer;
    };
}