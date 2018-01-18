#pragma once

#include <cstdint>

namespace engine {
    namespace layers {
        struct image_view {
            float index;
            std::uint16_t u, v;
        };
    }
}