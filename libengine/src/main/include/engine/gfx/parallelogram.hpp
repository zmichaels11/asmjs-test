#pragma once

#include "engine/gfx/point.hpp"

namespace engine {
    namespace gfx {
        template<typename point_component_T>
        struct parallelogram {
            engine::gfx::point<point_component_T> upperLeft, upperRight, bottomLeft;
        };
    }
}