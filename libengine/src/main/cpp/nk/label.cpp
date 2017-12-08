#include "nk/label.hpp"
#include "nk/text_align.hpp"

#include "nuklear/nk_ctx.hpp"

namespace nk {
    void label(const char* title, text_align align) {
        auto ctx = nk::getContext();

        nk_label(&ctx->context, title, static_cast<nk_text_alignment> (align));
    }
}