#pragma once

#include <cstddef>

#include "graphics/attachment_info.hpp"

namespace graphics {
    struct framebuffer_info {
        graphics::attachment_info * pAttachments;
        std::size_t attachmentCount;
    };
}