#pragma once

#include "graphics/tokens/header.hpp"

namespace graphics {
    namespace tokens {
        struct cmd_use_program {
            graphics::tokens::header header;
            unsigned int programID;

            inline static cmd_use_program create(unsigned int programID) noexcept {
                return {graphics::tokens::header::USE_PROGRAM, programID};
            }
        };
    }
}