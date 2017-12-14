#include "renderer/text_layer_info.hpp"

namespace renderer {
    template<>
    text_layer_info defaults<text_layer_info>() {         
        text_layer_info info;

        info.maxCharacters = 4096;
        info.firstChar = ' ';
        info.charCount = 96;
        info.pointSize = 16.0F;
        info.fontFile = "data/fonts/Roboto-Regular.ttf";

        return info;    
    }
}