#pragma once

namespace graphics {
    enum class compare_op : unsigned int {
        NEVER = 0x0200,
        ALWAYS = 0x0207,
        LESS = 0x0201,
        EQUAL = 0x0202,
        GREATER = 0x0204,
        LEQUAL = 0x0203,
        GEQUAL = 0x0206,
        NOTEQUAL = 0x0205
    };
}