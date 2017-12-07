#include "engine/gui/tooltip.hpp"

#include <string>

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void tooltip::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            if (nk_tooltip_begin(&nk->context, _width)) {
                nk_tooltip(&nk->context, _tip.c_str());
                nk_tooltip_end(&nk->context);
            }
        }

        const std::string& tooltip::getTip() const {
            return _tip;
        }

        float tooltip::getWidth() const {
            return _width;
        }
    }
}