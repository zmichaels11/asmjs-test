#pragma once

#include <string>

namespace graphics {
    struct font_info {
        char firstChar;
        unsigned int charCount;
        std::string fontFile;
        float charHeight;        
    };
}