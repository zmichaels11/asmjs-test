#pragma once

#include "graphics/hstate/state.hpp"

namespace graphics {
    struct pixel_store_state_info {
        struct pack_unpack_t {
            bool swapBytes;
            bool lsbFirst;
            int rowLength;
            int imageHeight;
            int skipPixels;
            int skipRows;
            int skipImages;
            int alignment;
        } pack, unpack;

        static pixel_store_state_info defaults() noexcept;
    };

    template<>
    void apply(const graphics::pixel_store_state_info& info) noexcept;
}