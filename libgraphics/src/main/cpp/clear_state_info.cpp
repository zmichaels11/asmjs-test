#include "graphics/hstate/clear_state_info.hpp"

#include "graphics/hbitfield/clear_buffer.hpp"

namespace graphics {    
    clear_state_info clear_state_info::defaults() noexcept {
        return {
            static_cast<clear_buffer> (0),
            {0.0F, 0.0F, 0.0F, 0.0F},
            1.0F,
            0
        };
    }
}
