#include "nk/window.hpp"

#include "nk/nk_ctx.hpp"

namespace nk {    
    window::window(const window_info& info) {
        auto ctx = nk::getContext();
        auto bounds = nk_rect(info.bounds.x, info.bounds.y, info.bounds.width, info.bounds.height);

        if (info.id) {
            _success = nk_begin_titled(&ctx->context, info.id, info.title, bounds, static_cast<nk_flags> (info.flags));
        } else {
            _success = nk_begin(&ctx->context, info.title, bounds, static_cast<nk_flags> (info.flags));            
        }
    }

    window::~window() {
        auto ctx = nk::getContext();        

        nk_end(&ctx->context);
    }

    window::operator bool() const {
        return _success;
    }
}