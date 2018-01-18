#pragma once

#include "engine/layers/clear_type.hpp"
#include "engine/layers/color.hpp"

namespace engine {
    namespace layers {
        struct clear_operation {
            clear_type type;
            color clearColor;
            unsigned int clearStencil;
            float clearDepth;
        };
    }
}