#pragma once

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class menubar {
        public:            
            virtual void build(void * ctx);

            virtual void add(const std::shared_ptr<menu>& menu);

            virtual void addAll(const std::vector<std::shared_ptr<menu>>& menus);

            virtual void remove(const std::shared_ptr<menu>& widget);

            virtual void removeAll();

            const std::vector<std::shared_ptr<menu>>& getMenus() const;
        };
    }
}