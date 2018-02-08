#pragma once

#include "engine/layers/nuklear/edit_flags.hpp"

namespace engine {
    namespace layers {
        namespace nuklear {
            namespace edit_types {                
                constexpr auto SIMPLE = edit_flags::ALWAYS_INSERT_MODE;
                constexpr auto FIELD = edit_flags::ALWAYS_INSERT_MODE | edit_flags::SELECTABLE | edit_flags::CLIPBOARD;
                constexpr auto BOX = edit_flags::ALWAYS_INSERT_MODE | edit_flags::SELECTABLE | edit_flags::MULTILINE | edit_flags::ALLOW_TAB | edit_flags::CLIPBOARD;
                constexpr auto EDITOR = edit_flags::SELECTABLE | edit_flags::MULTILINE | edit_flags::ALLOW_TAB | edit_flags::CLIPBOARD;
            }
        }        
    }
}