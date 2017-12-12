#include "nk/color_picker.hpp"

#include "nk/color.hpp"
#include "nk/nk_ctx.hpp"

namespace nk {
    nk::color color_picker(const nk::color& color) {
        auto ctx = nk::getContext();        
        auto in = nk_color{color.r, color.g, color.b, color.a};    
        auto out = nk_color_picker(&ctx->context, in, NK_RGBA);

        return {out.r, out.g, out.b, out.a};
    }
}