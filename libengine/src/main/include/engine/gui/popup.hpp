#pragma once

#include <memory>
#include <vector>

#include "engine/gui/container.hpp"
#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class popup : public virtual widget {
            std::vector<std::shared_ptr<widget>> _children;
            bool * _pClose;

        public:
            virtual void build(void * ctx);

            virtual void add(const std::shared_ptr<widget>& component);

            virtual void remove(const std::shared_ptr<widget>& component);

            virtual void addAll(const std::vector<std::shared_ptr<widget>>& widgets);

            virtual void removeAll();

            virtual const std::vector<std::shared_ptr<widget>>& getWidgets() const;
        };
    }
}