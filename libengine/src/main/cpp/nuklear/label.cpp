#include "engine/gui/label.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void label::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            nk_flags align;

            switch (_alignment) {
                case label_alignment::LEFT:
                    align = NK_TEXT_LEFT;
                    break;
                case label_alignment::CENTER:
                    align = NK_TEXT_CENTERED;
                    break;
                case label_alignment::RIGHT:
                    align = NK_TEXT_RIGHT;
                    break;
            }

            nk_label(&nk->context, _text.c_str(), align);
        }
    }
}