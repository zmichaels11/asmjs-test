#pragma once

#include "engine/layers/point.hpp"

namespace engine {
    namespace layers {
        template <typename point_component_T = float>
        struct parallelogram {
            point<point_component_T> upperLeft, upperRight, lowerLeft;
        };
    }
}