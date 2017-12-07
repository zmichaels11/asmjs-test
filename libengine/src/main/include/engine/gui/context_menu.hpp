#pragma once

#include <memory>
#include <vector>

#include "engine/gui/contextual_widget.hpp"

namespace engine {
    namespace gui {
        class context_menu {
            std::vector<std::shared_ptr<contextual_widget>> _widgets;
            const float _width;
            const float _height;

        public:
            context_menu(float width, float height) :
                _width(width),
                _height(height) {}

            virtual ~context_menu() {}

            virtual void build(void * ctx);

            virtual void add(const std::shared_ptr<contextual_widget>& widget);

            virtual void addAll(const std::vector<std::shared_ptr<contextual_widget>>& widgets);

            virtual void remove(const std::shared_ptr<contextual_widget>& widget);

            virtual void removeAll();

            virtual const std::vector<std::shared_ptr<contextual_widget>>& getWidgets() const;
        };
    }
}