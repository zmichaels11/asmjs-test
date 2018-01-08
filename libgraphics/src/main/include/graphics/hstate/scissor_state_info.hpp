#pragma once

#include "graphics/hstate/state.hpp"

namespace graphics {
    struct scissor_state_info {
        bool scissorTestEnabled;
        int left, bottom, width, height;

        static scissor_state_info defaults() noexcept;
    };

    template<>
    void apply(const graphics::scissor_state_info& info) noexcept;
}