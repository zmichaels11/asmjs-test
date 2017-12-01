#pragma once

#include <functional>

#include "engine/gui/color.hpp"
#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class color_combo_box : public virtual component {            
            color4ub _color;
            std::function<void(const color_combo_box *)> _onChange;

        public:
            color_combo_box(color4ub color = {0, 0, 0, 0}) :                
                _color(color),
                _onChange(nullptr) {}

            virtual ~color_combo_box() {}

            virtual void build();            

            inline color4ub getColor() const;

            virtual void setOnChange(const std::function<void(const color_combo_box *)>& callback);
        };

        color4ub color_combo_box::getColor() const {
            return _color;
        }
    }
}