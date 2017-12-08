#pragma once

namespace nk {
    enum class text_align : unsigned int {
        LEFT = 0x10 | 0x01,
        CENTERED = 0x10 | 0x02,
        right = 0x10 | 0x04
    };
}