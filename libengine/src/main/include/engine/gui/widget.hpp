#pragma once

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class widget : public virtual component {
        public:
            virtual void build(void *) = 0;
        };
    }
}