#pragma once

#include "engine.hpp"

namespace gui_test {
    class gui_component {
    public:
        virtual ~gui_component() {}

        virtual void handle(engine::layers::gui_layer * pLayer) noexcept = 0;
    };
}