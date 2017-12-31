#pragma once

namespace renderer {
    enum class render_target : unsigned int {
        COLOR = 1,
        NORMAL = 2,
        DEPTH = 3,
        STENCIL = 4
    };
}