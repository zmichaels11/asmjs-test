#pragma once

#include "graphics/henum/attachment_type.hpp"

namespace graphics {
    class renderbuffer;
    class texture;

    struct attachment_info {
        int level;
        attachment_type type;
        
        union {
            graphics::renderbuffer * pRenderbuffer;
            graphics::texture * pTexture;
        };

        attachment_info() noexcept {}

        constexpr attachment_info(graphics::renderbuffer * pRb) noexcept :
            level(0),
            type(attachment_type::RENDERBUFFER),
            pRenderbuffer(pRb) {}

        constexpr attachment_info(graphics::texture * pTex, int mipLevel = 0) noexcept :
            level(mipLevel),
            type(attachment_type::TEXTURE),
            pTexture(pTex) {}

    };        
}