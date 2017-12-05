#pragma once

#include <memory>
#include <vector>

#include "engine/gui/container.hpp"
#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class window : public virtual container {
            std::vector<std::shared_ptr<widget>> _children;

        public:
            virtual void build(void *);
            
            virtual void add(std::shared_ptr<widget> widget);

            virtual void addAll(const std::vector<std::shared_ptr<widget>>& widgets);

            virtual void remove(std::shared_ptr<widget> widget);
        };
    }
}