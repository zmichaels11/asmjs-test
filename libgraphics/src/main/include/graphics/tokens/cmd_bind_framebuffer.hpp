#pragma once

#include "graphics/tokens/header.hpp"

namespace graphics {
    namespace tokens {
        struct cmd_bind_framebuffer {
            graphics::tokens::header header;
            unsigned int framebufferID;
            int drawBuffers[8];            

            inline static cmd_bind_framebuffer create(unsigned int framebufferID, unsigned int drawBuffers) 
        };
    }
}