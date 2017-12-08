#include "nk/button.hpp"

#include "nuklear/nk_ctx.hpp"

namespace nk {
    bool button_label(const char* title) {
        auto ctx = nk::getContext();

        return nk_button_label(&ctx->context, title);
    }
    
    bool button_text(const char* title, int len) {
        auto ctx = nk::getContext();

        return nk_button_text(&ctx->context, title, len);
    }
}