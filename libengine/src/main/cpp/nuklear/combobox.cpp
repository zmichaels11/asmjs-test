#include "engine/gui/combobox.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void string_combobox::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto size = nk_vec2(nk_widget_width(&nk->context), 200);

            if (nk_combo_begin_label(&nk->context, _values[_selectedIdx].c_str(), size)) {            
                nk_layout_row_dynamic(&nk->context, 35, 1);
            
                int selected = _selectedIdx;

                for (int i = 0; i < _values.size(); i++) {
                    if (nk_combo_item_label(&nk->context, _values[i].c_str(), NK_TEXT_LEFT)) {
                        selected = i;
                    }
                }

                nk_combo_end(&nk->context);

                if (_selectedIdx != selected) {
                    _selectedIdx = selected;
                    trigger();
                }
            }
        }

        const std::string& string_combobox::getValue() const {
            return _values[_selectedIdx];
        }

        void string_combobox::setOnChange(const std::function<void(const string_combobox*)>& callback) {
            _onChange = callback;
        }

        void string_combobox::trigger() const {
            if (_onChange) {
                _onChange(this);
            }
        }

        void color_combobox::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto color = nk_rgba(_color.r, _color.g, _color.b, _color.a);      
            auto size = nk_vec2(nk_widget_width(&nk->context), 400);

            decltype(color) newColor;

            if (nk_combo_begin_color(&nk->context, color, size)) {
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

                    trigger();
                }

                nk_combo_end(&nk->context);
            }
        }  

        color4ub color_combobox::getColor() const {
            return _color;
        }      

        void color_combobox::setOnChange(const std::function<void(const color_combobox*)>& callback) {
            _onChange = callback;
        }

        void color_combobox::trigger() const {
            if (_onChange) {
                _onChange(this);
            }
        }
    }
}