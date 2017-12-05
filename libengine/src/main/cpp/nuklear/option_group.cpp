#include "engine/gui/option_group.hpp"

#include "nuklear/nk_ctx.hpp"

namespace engine {
    namespace gui {
        void option_group::build(void * ctx) {
            auto nk = reinterpret_cast<nk::nk_ctx*> (ctx);
            auto current = _selectedIdx;

            for (int idx = 0; idx < _pOptions.size(); idx++) {
                if (nk_option_label(&nk->context, _pOptions[idx]->label.c_str(), _selectedIdx == idx)) {
                    _selectedIdx = idx;
                }
            }

            if (current != _selectedIdx && _onChange) {
                _onChange(this);
            }
        }

        option_group::option_group(const option * pOptions, std::size_t count) {
            for (int i = 0; i < count; i++) {
                _pOptions.push_back(pOptions + i);
            }

            _onChange = nullptr;
            _selectedIdx = 0;
        }
    }
}