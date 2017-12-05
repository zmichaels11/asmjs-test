#pragma once

#include "engine/gui/component.hpp"

#include <memory>
#include <vector>

namespace engine {
    namespace gui {
        class widget;

        class container : public virtual component {
        public:
            virtual void build() = 0;

            virtual void add(std::shared_ptr<widget> component) = 0;

            virtual void addAll(const std::vector<std::shared_ptr<widget>>& widgets) = 0;
            
            virtual void remove(std::shared_ptr<widget> component) = 0;

            virtual void removeAll() = 0;

            virtual const std::vector<std::shared_ptr<widget>>& getWidgets() = 0;
        };
    }
}