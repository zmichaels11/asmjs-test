#pragma once

#include <string>

#include "engine/gui/alignment.hpp"
#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {        
        class label : public virtual widget {
            std::string _text;
            alignment _alignment;
        
        public:
            label(const std::string& text, alignment align = alignment::LEFT) :
                _text(text),
                _alignment(align) {}

            virtual ~label() {}

            virtual void build(void *);

            virtual const std::string& getLabel() const;

            virtual alignment getAlignment() const;
        };    
    }
}