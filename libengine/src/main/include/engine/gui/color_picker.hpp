#pragma once

#include "engine/gui/widget.hpp"

#include <cstdint>

#include <functional>

#include "engine/gui/color.hpp"

namespace engine {
    namespace gui {               
        class color_picker : public virtual widget {        
            color4ub _color;    
            std::function<void(const color_picker*)> _onChange;

        public:
            color_picker(const color4ub& color) :
                _color(color) {}

            virtual void build(void * ctx);

            virtual const color4ub& getColor() const;

            virtual void setOnChange(const std::function<void(const color_picker*)>& callback);

            virtual void trigger() const;
        };
    }
}