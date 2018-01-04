#pragma once

#include "graphics/info.hpp"

namespace graphics {
    struct scissor_state_info {
        bool scissorTestEnabled;
        int left, bottom, width, height;
    };

    void apply(const graphics::scissor_state_info& info);

    template<>
    scissor_state_info defaults();
}