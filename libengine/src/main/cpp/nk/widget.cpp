#include "nk/widget.hpp"

#include "nk/rect.hpp"
#include "nk/vec2.hpp"

#include "nuklear/nk_ctx.hpp"

namespace nk {
    namespace widget {
        float width() {
            auto ctx = nk::getContext();

            return nk_widget_width(&ctx->context);
        }

        float height() {
            auto ctx = nk::getContext();

            return nk_widget_height(&ctx->context);
        }

        bool isHovered() {
            auto ctx = nk::getContext();

            return nk_widget_is_hovered(&ctx->context);
        }

        rect bounds() {
            auto ctx = nk::getContext();
            auto out = nk_widget_bounds(&ctx->context);
            
            return {out.x, out.y, out.w, out.h};
        }

        vec2 position() {
            auto ctx = nk::getContext();
            auto out = nk_widget_position(&ctx->context);

            return {out.x, out.y};
        }

        vec2 size() {
            auto ctx = nk::getContext();
            auto out = nk_widget_size(&ctx->context);

            return {out.x, out.y};
        }
    }
}