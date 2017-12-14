#pragma once

#include <string>

namespace renderer {
    struct text_layer_info {
        unsigned int maxCharacters;
        int firstChar;
        unsigned int charCount;
        float pointSize;
        std::string fontFile;
    };

    inline void defaults(text_layer_info& info) {
        info.maxCharacters = 4096;
        info.firstChar = ' ';
        info.charCount = 96;
        info.pointSize = 16.0F;
        info.fontFile = "data/fonts/Roboto-Regular.ttf";
    }
}