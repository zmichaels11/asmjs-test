#pragma once

#include "graphics/tokens/header.hpp"

namespace graphics {
    namespace tokens {
        struct cmd_bind_texture {
            graphics::tokens::header header;
            unsigned int target;
            unsigned int binding;
            unsigned int textureID;

            inline static cmd_bind_texture create(unsigned int target, unsigned int binding, unsigned int textureID) noexcept {
                return {graphics::tokens::header::BIND_TEXTURE, target, binding, textureID};
            }
        };
    }
}