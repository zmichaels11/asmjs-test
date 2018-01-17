#pragma once

#include <memory>
#include <string>

#include "engine/layers/base_resources.hpp"
#include "engine/layers/image_view.hpp"
#include "engine/layers/sprite_sheet_info.hpp"

namespace engine {
    namespace layers {        
        class sprite_sheet {
            std::unique_ptr<engine::layers::base_resources> _pResources;

            sprite_sheet(const sprite_sheet&) = delete;

            sprite_sheet& operator=(const sprite_sheet&) = delete;

        public:
            sprite_sheet(sprite_sheet&&) = default;

            sprite_sheet& operator=(sprite_sheet&&) = default;

            sprite_sheet() noexcept : _pResources(nullptr) {}

            sprite_sheet(const engine::layers::sprite_sheet_info& info) noexcept;

            const engine::layers::image_view& getSprite(const std::string& lookup) const noexcept;

            const engine::layers::sprite_sheet_info& getInfo() const noexcept;

            const void * getTexture() const noexcept;
        };
    }
}