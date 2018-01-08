#pragma once

namespace graphics {
    enum class stencil_op : unsigned int {
        KEEP = 0x1E00,
        ZERO = 0,
        REPLACE = 0x1E01,
        INCR = 0x1E02,
        DECR = 0x1E03,
        INVERT = 0x150A,
        INCR_WRAP = 0x8507,
        DECR_WRAP = 0x8508
    };
}