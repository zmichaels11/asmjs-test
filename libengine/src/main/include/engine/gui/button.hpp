#pragma once

#include <functional>
#include <string>

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class button : public virtual component {
            std::string _label;
            std::function<void(const button *)> _onAction;

        public:
            button(const std::string& label) : 
                _label(label),
                _onAction(nullptr) {}

            virtual ~button() {}

            virtual void build();

            void setOnAction(const std::function<void(const button *)>& callback);

            inline const std::string& getLabel() const;
        };

        const std::string& button::getLabel() const {
            return _label;
        }
    }
}