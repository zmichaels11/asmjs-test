#pragma once

#include "nk/edit_flag.hpp"

namespace nk {
    enum class edit_type : unsigned int {
        SIMPLE = static_cast<unsigned int> (edit_flag::ALWAYS_INSERT_MODE),
        FIELD = static_cast<unsigned int> (edit_flag::ALWAYS_INSERT_MODE | edit_flag::SELECTABLE | edit_flag::CLIPBOARD),
        BOX = static_cast<unsigned int> (edit_flag::ALWAYS_INSERT_MODE | edit_flag::SELECTABLE | edit_flag::MULTILINE | edit_flag::ALLOW_TAB | edit_flag::CLIPBOARD),
        EDITOR = static_cast<unsigned int> (edit_flag::SELECTABLE | edit_flag::MULTILINE | edit_flag::ALLOW_TAB | edit_flag::CLIPBOARD)
    };    
}