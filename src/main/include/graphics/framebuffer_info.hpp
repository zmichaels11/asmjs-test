#pragma once

#include <cstddef>

namespace graphics {
    struct attachment_info;

    struct framebuffer_info {
        attachment_info * pAttachments;
        std::size_t attachmentCount;
    };
}