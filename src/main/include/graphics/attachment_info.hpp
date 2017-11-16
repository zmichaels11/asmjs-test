#pragma once

namespace graphics {
    class renderbuffer;
    class texture;

    struct attachment_info {
        int level;
        renderbuffer * pRenderbuffer;
        texture * pTexture;
    };
}