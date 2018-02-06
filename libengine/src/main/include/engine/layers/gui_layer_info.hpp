#pragma once

#include <cstddef>

#include <string>

namespace engine {
    namespace layers {
        struct gui_layer_info {
            struct font_info {
                std::string file;
                float height;
            } * pFonts;

            std::size_t nFonts;
            int defaultFontID;
        };
    }
}