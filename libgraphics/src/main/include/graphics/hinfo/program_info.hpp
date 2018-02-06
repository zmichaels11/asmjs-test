#pragma once

#include <cstddef>

#include "graphics/hinfo/attribute_state_info.hpp"

namespace graphics {
    class shader;

    struct program_info {
        shader ** ppShaders;
        std::size_t nShaders;
        attribute_state_info * pAttributes;
        std::size_t nAttributes;
        unsigned int uuid;        
    };
}