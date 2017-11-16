#pragma once

namespace graphics {
    enum class buffer_target : unsigned int {
        ARRAY = 0x8892,
        ELEMENT = 0x8893,
        PIXEL_UNPACK = 0x88EC,
        PIXEL_PACK = 0x88EB,
        COPY_READ = 0x8F36,
        COPY_WRITE = 0x8F37,
        TRANSFORM_FEEDBACK = 0x8C8E,
        UNIFORM_BUFFER = 0x8A11
    };
}