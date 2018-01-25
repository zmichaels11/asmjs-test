#pragma once

#include "engine/layers/image_filter_type.hpp"
#include "engine/layers/image_scroll_type.hpp"
#include "engine/layers/tile_offset_type.hpp"

namespace engine {
    namespace layers {
        struct tiled_image_info {
            struct dimensions_t {
                unsigned int columns;
                unsigned int rows;
            } dim;

            struct tile_size_t {
                unsigned int width;
                unsigned int height;
            } tileSize;

            struct scroll_t {
                image_scroll_type horizontal;
                image_scroll_type vertical;
            } scroll;

            image_filter_type filter;
            int tileSheetID;

            inline int index(int col, int row) const noexcept;
        };

        int tiled_image_info::index(int col, int row) const noexcept {
            return dim.columns * row + col;
        }
    }
}