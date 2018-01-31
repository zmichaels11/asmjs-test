#pragma once

#include "engine/layers/point.hpp"

namespace engine {
    namespace layers {
        template <typename point_component_T = float>
        struct parallelogram {
            point<point_component_T> upperLeft, upperRight, lowerLeft;

            static constexpr parallelogram<point_component_T> quad(
                point_component_T x, 
                point_component_T y, 
                point_component_T w, 
                point_component_T h) noexcept {
                    
                return {
                    {x, y},
                    {x + w, y},
                    {x, y + h}};
            }
        };
    }
}