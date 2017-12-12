#pragma once

namespace renderer {
    struct rgba {
        float red, green, blue, alpha;
    };    

    inline void black(rgba& c) {
        c.red = 0.0F;
        c.green = 0.0F;
        c.blue = 0.0F;
        c.alpha = 1.0F;
    }

    inline void transparentBlack(rgba& c) {
        c.red = 0.0F;
        c.green = 0.0F;
        c.blue = 0.0F;
        c.alpha = 0.0F;
    }

    inline void white(rgba& c) {
        c.red = 1.0F;
        c.green = 1.0F;
        c.blue = 1.0F;
        c.alpha = 1.0F;
    }

    inline void identityOffset(rgba& c) {
        transparentBlack(c);
    }

    inline void identityScale(rgba& c) {
        white(c);
    }
}