#include "graphics/clear_state_info.hpp"

#include "graphics/clear_buffer.hpp"

namespace graphics {
    template<>
    clear_state_info defaults() {
        return {
            static_cast<clear_buffer> (0),
            {0.0F, 0.0F, 0.0F, 0.0F},
            1.0F,
            0
        };
    }
}