#pragma once

#include <cstddef>

#include <functional>
#include <string>
#include <vector>

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        struct option {
            std::string label;
            const void * userData;
        };

        class option_group : public virtual component {
            std::vector<const option *> _pOptions;
            std::function<void(const option_group*)> _onChange;
            int _selectedIdx;

        public:
            option_group(const std::vector<const option*> pOptions) :
                _pOptions(pOptions),
                _onChange(nullptr),
                _selectedIdx(0) {}

            option_group(const option * pOptions, std::size_t count);

            virtual ~option_group() {}

            virtual void build();

            inline const option * getSelected() const;

            inline int getSelectedIndex() const;

            inline void setSelectedIndex(int idx);

            inline void setSelected(const option * opt);

            inline void setOnChange(const std::function<void(const option_group *)>& callback);
        };

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