#pragma once

namespace graphics {
    enum class draw_mode : unsigned int {
        POINTS = 0x0000,
        LINE_STRIP = 0x0003,
        LINE_LOOP = 0x0002,
        LINES = 0x0001,
        TRIANGLE_STRIP = 0x0005,
        TRIANGLE_FAN = 0x0006,
        TRIANGLES = 0x0004
    };
}

