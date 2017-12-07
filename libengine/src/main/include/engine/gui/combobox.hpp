#pragma once

#include <functional>
#include <string>
#include <vector>

#include "engine/gui/color.hpp"
#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class string_combobox : public virtual widget {
            const std::vector<std::string> _values;
            int _selectedIdx;
            std::function<void(const string_combobox *)> _onChange;
        
        public:
            string_combobox(const std::vector<std::string>& values, int selected = 0) :
                _values(values),
                _selectedIdx(selected),
                _onChange(nullptr) {}

            virtual ~string_combobox() {}

            virtual void build(void * ctx);

            virtual const std::string& getValue() const;

            virtual void setOnChange(const std::function<void(const string_combobox*)>& callback);

            virtual void trigger() const;
        };

        class color_combobox : public virtual widget {            
            color4ub _color;
            std::function<void(const color_combobox *)> _onChange;

        public:
            color_combobox(color4ub color = {0, 0, 0, 0}) :                
                _color(color),
                _onChange(nullptr) {}

            virtual ~color_combobox() {}

            virtual void build(void * ctx);            

            virtual color4ub getColor() const;

            virtual void setOnChange(const std::function<void(const color_combobox *)>& callback);

            virtual void trigger() const;
        };
    }
}