#pragma once

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class menu_widget {
        public:
            virtual void build(void * ctx) = 0;
        };
    }
}