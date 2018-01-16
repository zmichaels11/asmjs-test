#pragma once

namespace engine {
    namespace gfx {
        enum class combine_mode {
            REPLACE,
            INTERSECT,
            UNION,
            XOR,
            EXCLUDE,
            COMPLEMENT
        };
    }
}