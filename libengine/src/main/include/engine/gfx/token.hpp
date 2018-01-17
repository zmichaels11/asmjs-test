#pragma once

#include <cstddef>

#include "engine/gfx/token_type.hpp"

namespace engine {
    namespace gfx {
        struct token {
            token_type type;
            std::size_t maxCount;
        };
    }
}