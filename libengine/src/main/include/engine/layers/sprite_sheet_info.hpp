#pragma once

#include <set>
#include <string>

#include "engine/layers/sprite_sheet_hint.hpp"

namespace engine {
    namespace layers {
        struct sprite_sheet_info {
            std::set<std::string> requiredImages;
            engine::layers::sprite_sheet_hint hints;
        };
    }
}