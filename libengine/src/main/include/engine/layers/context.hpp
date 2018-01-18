#pragma once

#include <memory>

#include "engine/layers/base_resources.hpp"
#include "engine/layers/context_info.hpp"
#include "engine/layers/sprite_sheet.hpp"

namespace engine {
    namespace layers {
        class context {
            std::unique_ptr<base_resources> _pResources;

            context(const context&) = delete;

            context& operator=(const context&) = delete;

        public:
            context(context&&) = default;

            context& operator=(context&&) = default;

            context(const context_info& info) noexcept;

            const sprite_sheet * getSpriteSheet(int id) const noexcept;

            const context_info& getInfo() const noexcept;
        };
    }
}