#pragma once

#include <cstddef>

#include "graphics/hinfo/attachment_info.hpp"

namespace graphics {
    struct framebuffer_info {
        graphics::attachment_info * pAttachments;
        std::size_t nAttachments;
    };
}