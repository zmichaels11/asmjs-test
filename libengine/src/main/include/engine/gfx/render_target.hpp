#pragma once

namespace engine {
    namespace gfx {
        enum class render_target : unsigned int {
            COLOR = 1,
            STENCIL = 2,
            DEPTH = 4
        };
    }
}