#pragma once

#include "nk/rect.hpp"
#include "nk/vec2.hpp"

namespace nk {
    namespace widget {
        float width();

        float height();

        rect bounds();

        bool isHovered();

        vec2 position();

        vec2 size();
    }
}