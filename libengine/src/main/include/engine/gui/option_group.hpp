#pragma once

#include <cstddef>

#include <functional>
#include <string>
#include <vector>

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        struct option {
            std::string label;
            const void * userData;
        };

        class option_group : public virtual widget {
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

            virtual void build(void *);

            virtual const option * getSelected() const;

            virtual int getSelectedIndex() const;

            virtual void setSelectedIndex(int idx);

            virtual void setSelected(const option * opt);

            virtual void setOnChange(const std::function<void(const option_group *)>& callback);
        };        
    }
}