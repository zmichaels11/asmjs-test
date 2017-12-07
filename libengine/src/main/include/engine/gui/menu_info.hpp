#pragma once

#include <cstddef>

#include <string>

#include "engine/gui/symbol_type.hpp"

namespace engine {
    namespace gui {
        struct menu_label_info {
            std::string title;
            struct size_t {
                float width;
                float height;
            } size;
        };

        struct menu_symbol_info {
            symbol_type type;
            struct size_t {
                float width;
                float height;
            } size;
        };

        struct menu_text_info {
            const char * title;
            std::size_t length;
            struct size_t {
                float width;
                float height;
            } size;
        };        
    }
}