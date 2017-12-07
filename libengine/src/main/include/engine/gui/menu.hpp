#pragma once

#include "engine/gui/container.hpp"

#include <memory>
#include <vector>

namespace engine {
    namespace gui {
        template<typename info_t>
        class menu {
            const info_t _info;
            std::vector<std::shared_ptr<menu_widget>> _widgets;

        public:
            virtual void build(void * ctx);

            virtual void add(const std::shared_ptr<menu_widget>& widget);

            virtual void addAll(const std::vector<std::shared_ptr<menu_widget>>& widgets);

            virtual void remove(const std::shared_ptr<menu_widget>& widget);

            virtual void removeAll();

            const std::vector<std::shared_ptr<menu_widget>>& getWidgets() const;
        };
    }
}