#pragma once

#include <memory>
#include <vector>

#include "engine/gui/widget.hpp"
#include "engine/gui/window_info.hpp"

namespace engine {
    namespace gui {
        class window {
            std::vector<std::shared_ptr<widget>> _children;
            const window_info _info;
            void * _ctx;

        public:
            window(const window_info& info) :
                _children(),
                _info(info) {}

            virtual ~window() {}

            virtual void build(void *);
            
            virtual void add(const std::shared_ptr<widget>& widget);

            virtual void addAll(const std::vector<std::shared_ptr<widget>>& widgets);

            virtual void remove(const std::shared_ptr<widget>& widget);

            virtual void removeAll();

            virtual const std::vector<std::shared_ptr<widget>>& getWidgets() const;

            virtual void show();

            virtual void hide();

            virtual void setPosition(float x, float y);

            virtual void setSize(float x, float y);            
        };
    }
}