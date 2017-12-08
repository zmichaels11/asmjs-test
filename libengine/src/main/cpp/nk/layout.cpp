#include "nk/layout.hpp"

#include "nuklear/nk_ctx.hpp"

namespace nk {
    namespace layout {
        void row_static(float height, float width, int cols) {
            auto ctx = nk::getContext();

            nk_layout_row_static(&ctx->context, height, width, cols);
        }

        void row_dynamic(float height, int cols) {
            auto ctx = nk::getContext();

            nk_layout_row_dynamic(&ctx->context, height, cols);
        }
    }
}