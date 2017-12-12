#pragma once

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
    };

    void apply(const graphics::pixel_store_state_info& info);
}