#pragma once

#include "renderer/hsv.hpp"
#include "renderer/rgba.hpp"

namespace renderer {
    struct color_transform {
        renderer::hsv hsv;
        renderer::rgba rgbaOffset, rgbaScale;
    };    

    inline void identity(color_transform& ct) noexcept {
        identity(ct.hsv);
        identityOffset(ct.rgbaOffset);
        identityScale(ct.rgbaScale);
    }
}