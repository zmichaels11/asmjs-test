#pragma once

#include "graphics/tokens/header.hpp"

namespace graphics {
    namespace tokens {
        struct cmd_push_constant1i {
            graphics::tokens::header header;
            int location;
            int value;

            inline static cmd_push_constant1i create(int location, int value) noexcept {
                return {graphics::tokens::header::PUSH_CONSTANT1I, location, value};
            }
        };
    }
}