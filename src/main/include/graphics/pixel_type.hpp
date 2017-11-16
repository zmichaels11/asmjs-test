#pragma once

namespace graphics {
    enum class pixel_type : unsigned int {
        UNSIGNED_BYTE = 0x1401,
        UNSIGNED_SHORT = 0x1403,
        UNSIGNED_INT = 0x1405,
        BYTE = 0x1400,
        SHORT = 0x1402,
        INT = 0x1404,
        HALF_FLOAT = 0x140B,
        FLOAT = 0x1406,
        UNSIGNED_SHORT_5_6_5 = 0x8363,
        UNSIGNED_INT_5_9_9_9_REV = 0x8C3E,
        UNSIGNED_SHORT_5_5_5_1 = 0x8034,
        UNSIGNED_INT_2_10_10_10_REV = 0x8368,
        UNSIGNED_SHORT_4_4_4_4 = 0x8033        
    };
}