#include "nk/combobox.hpp"

#include "nk/nk_ctx.hpp"

namespace nk {
    combobox_color::combobox_color(const combobox_color_info& info) {
        auto ctx = nk::getContext();
        auto color = nk_color{info.color.r, info.color.g, info.color.b, info.color.a};
        auto size = nk_vec2(info.size.x, info.size.y);

        _selected = nk_combo_begin_color(&ctx->context, color, size);
    }

    combobox_color::~combobox_color() {
        if (_selected) {
            auto ctx = nk::getContext();

            nk_combo_end(&ctx->context);
        }
    }

    combobox_color::operator bool() const {
        return _selected;
    }
}