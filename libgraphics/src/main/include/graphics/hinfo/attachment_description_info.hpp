#pragma once

#include "graphics/henum/attachment_loadop.hpp"
#include "graphics/henum/draw_buffer.hpp"

namespace graphics {
    struct attachment_description_info {
        attachment_loadop loadOp;
        draw_buffer attachment;
    };
}