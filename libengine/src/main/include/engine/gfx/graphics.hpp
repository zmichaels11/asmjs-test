#pragma once

#include <memory>
#include <vector>

#include "engine/gfx/color.hpp"

namespace engine {
    namespace gfx {
        class graphics {            
            std::vector<std::shared_ptr<void*>>
        public:            
            void clearColor(const engine::gfx::color& color) noexcept;
            

            
        };
    }
}