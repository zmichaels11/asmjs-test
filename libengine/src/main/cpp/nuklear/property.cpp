#include "engine/gui/property.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        template<>
        void property<int>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto current = _value;
            
            nk_property_int(&nk->context, _label.c_str(), _min, &_value, _max, _step, _incPerPixel);

            if (current != _value) {
                trigger();
            }
        }

        template<>
        void property<float>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto current = _value;
            
            nk_property_float(&nk->context, _label.c_str(), _min, &_value, _max, _step, _incPerPixel);

            if (current != _value) {
                trigger();
            }
        }

        template<>
        void property<double>::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto current = _value;
            
            nk_property_double(&nk->context, _label.c_str(), _min, &_value, _max, _step, _incPerPixel);

            if (current != _value) {
                trigger();
            }
        }
    }
}