#include "engine/gui/static_row_layout.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void static_row_layout::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_layout_row_static(&nk->context, _height, _itemWidth, _cols);
        }
    }
}