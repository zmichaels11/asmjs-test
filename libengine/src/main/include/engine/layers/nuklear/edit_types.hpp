#pragma once

#include "engine/layers/nuklear/edit_flags.hpp"

namespace engine {
    namespace layers {
        namespace nuklear {
            enum class edit_types : edit_flags {
                SIMPLE = edit_flags::ALWAYS_INSERT_MODE,
                FIELD = edit_flags::ALWAYS_INSERT_MODE | edit_flags::SELECTABLE | edit_flags::CLIPBOARD,
                BOX = edit_flags::ALWAYS_INSERT_MODE | edit_flags::SELECTABLE | edit_flags::MULTILINE | edit_flags::ALLOW_TAB | edit_flags::CLIPBOARD,
                EDITOR = edit_flags::SELECTABLE | edit_flags::MULTILINE | edit_flags::ALLOW_TAB | edit_flags::CLIPBOARD
            };
        }
    }
}