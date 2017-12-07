#include "engine/gui/color_picker.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        namespace {
            bool compare(nk_color lhs, nk_color rhs) {
                return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
            }
        }

        void color_picker::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto curColor = nk_color{_color.r, _color.g, _color.b, _color.a};
            auto newColor = nk_color_picker(&nk->context, curColor, NK_RGBA);

            if (!compare(newColor, curColor)) {
                _color = {newColor.r, newColor.g, newColor.b, newColor.a};
                trigger();
            }            
        }

        void color_picker::trigger() const {
            if (_onChange) {
                _onChange(this);
            }
        }

        const color4ub& color_picker::getColor() const {
            return _color;
        }

        void color_picker::setOnChange(const std::function<void(const color_picker*)>& callback) {
            _onChange = callback;
        }
    }
}