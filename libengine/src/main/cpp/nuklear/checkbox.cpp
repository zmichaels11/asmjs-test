#include "engine/gui/checkbox.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void checkbox::setOnChange(const std::function<void(const checkbox*)>& callback) {
            _onChange = callback;
        }

        const std::string& checkbox::getLabel() const {
            return _label;
        }

        void checkbox::trigger() const {
            if (_onChange) {
                _onChange(this);
            }
        }

        bool checkbox::isActive() const {
            return _active;
        }

        void checkbox::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            int current = _active ? 1 : 0;            

            nk_checkbox_label(&nk->context, _label.c_str(), &current);

            if (_onChange && _active != current) {
                _active = current;
                _onChange(this);                
            }
        }
    }
}