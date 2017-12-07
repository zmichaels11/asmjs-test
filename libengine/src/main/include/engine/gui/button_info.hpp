#pragma once

#include <cstddef>
#include <cstdint>

#include <string>

#include "engine/gui/symbol_type.hpp"

namespace engine {
    namespace gui {
        struct button_color_info {
            std::uint8_t r, g, b, a;
        };        

        struct button_label_info {
            std::string title;
        };        

        struct button_symbol_info {
            symbol_type type;
        };

        struct button_text_info {
            const char * title;
            std::size_t length;
        };
    }
}