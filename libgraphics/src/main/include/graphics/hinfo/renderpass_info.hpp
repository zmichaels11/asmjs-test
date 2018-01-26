#pragma once

#include <cstddef>

#include "graphics/henum/attachment_loadop.hpp"
#include "graphics/hinfo/attachment_description_info.hpp"

namespace graphics {
    struct renderpass_info {
        attachment_description_info * pAttachments;
        std::size_t nAttachments;
    };
}