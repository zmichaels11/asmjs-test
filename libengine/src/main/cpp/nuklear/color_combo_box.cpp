#include "engine/gui/color_combo_box.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void color_combo_box::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto color = nk_rgba(_color.r, _color.g, _color.b, _color.a);            
            decltype(color) newColor;

            if (nk_combo_begin_color(&nk->context, color, nk_vec2(nk_widget_width(&nk->context), 400))) {
                nk_layout_row_dynamic(&nk->context, 120, 1);
                newColor = nk_color_picker(&nk->context, color, NK_RGBA);
                nk_layout_row_dynamic(&nk->context, 25, 1);
                newColor.r = static_cast<nk_byte> (nk_propertyi(&nk->context, "#R:", 0, newColor.r, 255, 1, 1));
                newColor.g = static_cast<nk_byte> (nk_propertyi(&nk->context, "#G:", 0, newColor.g, 255, 1, 1));
                newColor.b = static_cast<nk_byte> (nk_propertyi(&nk->context, "#B:", 0, newColor.b, 255, 1, 1));
                newColor.a = static_cast<nk_byte> (nk_propertyi(&nk->context, "#A:", 0, newColor.a, 255, 1, 1));

                if (color.r != newColor.r || color.g != newColor.g || color.b != newColor.b || color.a != newColor.a) {
                    _color.r = static_cast<std::uint8_t> (newColor.r);
                    _color.g = static_cast<std::uint8_t> (newColor.g);
                    _color.b = static_cast<std::uint8_t> (newColor.b);
                    _color.a = static_cast<std::uint8_t> (newColor.a);

                    if (_onChange) {
                        _onChange(this);
                    }
                }

                nk_combo_end(&nk->context);
            }
        }        
    }
}