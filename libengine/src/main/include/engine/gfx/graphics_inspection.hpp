#pragma once

#include <set>
#include <string>
#include <vector>

namespace engine {
    namespace gfx {
        struct graphics_inspection {
            std::vector<engine::gfx::token> tokens;
            std::set<std::string> images;
        };
    }
}