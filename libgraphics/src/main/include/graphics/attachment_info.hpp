#pragma once

namespace graphics {
    class renderbuffer;
    class texture;

    struct attachment_info {
        int level;
        graphics::renderbuffer * pRenderbuffer;
        graphics::texture * pTexture;
    };
}