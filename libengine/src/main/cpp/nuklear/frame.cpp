#include "engine/gui/frame.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void frame::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (ctx);

            if (nk_begin(&nk->context, _title.c_str(), nk_rect(_x, _y, _w, _h), static_cast<nk_flags> (_opts))) {
                for (auto&& child : _children) {
                    child->build(ctx);
                }
            }

            nk_end(&nk->context);
        }
    }
}