#pragma once

#include <cstdint>

namespace renderer {

    // sizeof(image_view) == 12
    struct image_view {
        std::int32_t index;
        std::uint16_t left, top, right, bottom;
    };

    //NOTE: index is an int32_t so the struct can be word aligned.

    inline void setBlank(image_view& view) {
        view.index = -1;
        view.left = view.right = view.top = view.bottom = 0;
    }
}