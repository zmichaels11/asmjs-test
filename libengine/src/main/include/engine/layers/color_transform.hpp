#pragma once

#include "math/mat4.hpp"
#include "math/vec4.hpp"

namespace engine {
    namespace layers {
        struct color_transform {
            math::mat4 rgbTransform;
            math::vec4 rgbaOffset;
        };
    }
}