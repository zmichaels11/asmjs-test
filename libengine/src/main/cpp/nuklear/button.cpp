#include "engine/gui/button.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void button::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx *> (ctx);

            if (nk_button_label(&nk->context, _label.c_str()) && _onAction) {
                _onAction(this);
            }
        }
    }
}