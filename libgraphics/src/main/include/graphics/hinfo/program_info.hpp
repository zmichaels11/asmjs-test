#pragma once

#include <cstddef>

#include "graphics/hinfo/attribute_state_info.hpp"

namespace graphics {
    class shader;

    struct program_info {
        shader ** ppShaders;
        std::size_t shaderCount;
        attribute_state_info * pAttributeStates;
        std::size_t attributeCount;
        unsigned int uuid;
    };
}