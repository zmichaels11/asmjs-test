#pragma once

namespace graphics {
    enum class shader_type : unsigned int {
        VERTEX = 0x8B31,
        FRAGMENT = 0x8B30,
        GEOMETRY = 0x8DD9,
        TESS_EVALUATION = 0x8E87,
        TESS_CONTROL = 0x8E88,
        COMPUTE = 0x91B9
    };
}