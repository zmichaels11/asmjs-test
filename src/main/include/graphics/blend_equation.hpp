#pragma once

namespace graphics {
    enum class blend_equation : unsigned int {
        ADD = 0x8006,
        SUBTRACT = 0x800A,
        REVERSE_SUBTRACT = 0x800B,
        MIN = 0x8007,
        MAX = 0x8008
    };
}