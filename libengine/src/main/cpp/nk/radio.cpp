#include "nk/radio.hpp"

#include "nk/nk_ctx.hpp"

namespace nk {
    bool radio_label(const char* title, bool* pActive) {
        auto ctx = nk::getContext();
        auto active = static_cast<int> (*pActive);
        auto out =  nk_radio_label(&ctx->context, title, &active);

        *pActive = active;
        return out;
    }

    bool radio_label(const char* title, bool active) {
        auto ctx = nk::getContext();

        return nk_option_label(&ctx->context, title, active);
    }
}