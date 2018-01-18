#pragma once

namespace engine {
    namespace layers {
        enum class write_hint : unsigned int {
            ONCE = 2,
            SOMETIMES = 1,
            OFTEN = 0
        };
    }
}