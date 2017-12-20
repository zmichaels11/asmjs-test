#include "graphics/pixel_store_state_info.hpp"

namespace graphics {
    template<>
    pixel_store_state_info defaults() {
        return {
                false,
                false,
                0,
                0,
                0,
                0,
                0,
                4,
                false,
                false,
                0,
                0,
                0,
                0,
                0,
                4
        };
    }
}