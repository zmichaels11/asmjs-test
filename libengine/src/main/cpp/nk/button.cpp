#include "nk/button.hpp"

#include "nuklear/nk_ctx.hpp"

namespace nk {
    button_label::button_label(const char * title) {
        auto ctx = nk::getContext();

        _success = nk_button_label(&ctx->context, title);
    }

    button_label::operator bool() const {
        return _success;
    }
}