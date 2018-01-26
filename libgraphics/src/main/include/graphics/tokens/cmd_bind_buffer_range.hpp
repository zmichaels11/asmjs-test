#pragma once

#include "graphics/tokens/header.hpp"

namespace graphics {
    namespace tokens {
        struct cmd_bind_buffer_range {
            graphics::tokens::header header;
            unsigned int target;
            unsigned int bufferID;
            unsigned int binding;
            long offset;
            long size;

            inline static cmd_bind_buffer_range create(
                unsigned int target, 
                unsigned int bufferID, 
                unsigned int binding, 
                long offset, 
                long range) noexcept {

                return {
                    graphics::tokens::header::BIND_BUFFER_RANGE,
                    target,
                    bufferID,
                    binding,
                    offset,
                    range};
            }
        };
    }
}