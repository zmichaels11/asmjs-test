#include "engine/gui/dynamic_row_layout.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void dynamic_row_layout::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);

            nk_layout_row_dynamic(&nk->context, _height, _cols);
        }

        float dynamic_row_layout::getHeight() const {
            return _height;
        }

        int dynamic_row_layout::getColumns() const {
            return _cols;
        }
    }
}