#include "engine/gui/slider.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        template<>
        void slider<float>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto current = _value;

            nk_slider_float(&nk->context, _min, &_value, _max, _step);

            if (_value != current && _onChange) {
                _onChange(this);
            }
        }

        template<>
        void slider<int>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto current = _value;

            nk_slider_int(&nk->context, _min, &_value, _max, _step);

            if (_value != current && _onChange) {
                _onChange(this);
            }
        }
    }
}