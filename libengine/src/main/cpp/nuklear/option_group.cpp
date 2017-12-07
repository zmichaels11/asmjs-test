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

        const option * option_group::getSelected() const {
            return _pOptions[_selectedIdx];
        }

        int option_group::getSelectedIndex() const {
            return _selectedIdx;
        }

        void option_group::setSelectedIndex(int idx) {
            _selectedIdx = idx;
        }

        void option_group::setSelected(const option * opt) {
            for (int i = 0; i < _pOptions.size(); i++) {
                if (_pOptions[i] == opt) {
                    _selectedIdx = i;
                    break;
                }
            }
        }

        void option_group::setOnChange(const std::function<void(const option_group *)>& callback) {
            _onChange = callback;
        }
    }
}