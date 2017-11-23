#pragma once

#include <cstddef>

namespace graphics {
    class buffer;
    class vertex_attribute_description;
    class vertex_binding_description;

    struct vertex_array_info {
        vertex_attribute_description * pAttributes;
        std::size_t attributeCount;
        vertex_binding_description * pBindings;
        std::size_t bindingCount;
        buffer * pIndexBuffer;
    };
}