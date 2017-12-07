#include "engine/gui/context_menu.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void context_menu::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            if (nk_contextual_begin(&nk->context, NK_WINDOW_NO_SCROLLBAR, nk_vec2(_width, _height), nk_window_get_bounds(&nk->context))) {
                nk_layout_row_dynamic(&nk->context, 30, 1);

                for (auto&& widget : _widgets) {
                    widget->build(ctx);
                }

                nk_contextual_end(&nk->context);
            }
        }
    }
}