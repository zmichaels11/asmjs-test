#pragma once

#include <cstddef>

namespace graphics {
    class shader;
    class attribute_state_info;

    struct program_info {
        shader * pShaders;
        std::size_t shaderCount;
        attribute_state_info * pAttributeStates;
        std::size_t attributeCount;
    };
}