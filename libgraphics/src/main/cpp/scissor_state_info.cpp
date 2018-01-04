#include "graphics/scissor_state_info.hpp"

namespace graphics {
    template<>
    scissor_state_info defaults() {
        return {false};
    }
}