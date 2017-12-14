#pragma once

#include <string>

#include "renderer/info.hpp"

namespace renderer {
    struct text_layer_info {
        unsigned int maxCharacters;
        int firstChar;
        unsigned int charCount;
        float pointSize;
        std::string fontFile;
    };

    template<>
    text_layer_info defaults<text_layer_info>();
}