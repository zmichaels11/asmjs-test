#pragma once

namespace graphics {
    struct scissor_state_info {
        bool scissorTestEnabled;
        int left, bottom, width, height;
    };

    void apply(const scissor_state_info& info);
}