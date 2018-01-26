#pragma once

#include "graphics/henum/draw_mode.hpp"

namespace graphics {
    struct input_assembly_state_info {
        draw_mode topology;
        bool primitiveRestartEnable;
    };
}