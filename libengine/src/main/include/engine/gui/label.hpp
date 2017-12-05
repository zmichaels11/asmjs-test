#pragma once

#include <string>

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        enum class label_alignment {
            LEFT,
            CENTER,
            RIGHT
        };

        class label : public virtual component {
            std::string _text;
            label_alignment _alignment;
        
        public:
            label(const std::string& text, label_alignment align = label_alignment::LEFT) :
                _text(text),
                _alignment(align) {}

            virtual ~label() {}

            virtual void build(void *);

            inline const std::string& getLabel() const;

            inline label_alignment getAlignment() const;
        };

        const std::string& label::getLabel() const {
            return _text;
        }

        label_alignment label::getAlignment() const {
            return _alignment;
        }
    }
}