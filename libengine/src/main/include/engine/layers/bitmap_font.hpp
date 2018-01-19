#pragma once

#include <memory>

#include "graphics/image.hpp"

#include "engine/layers/base_resources.hpp"

namespace engine {
    namespace layers {
        class bitmap_font {
            std::unique_ptr<base_resources> _pResources;

            bitmap_font(const bitmap_font&) = delete;

            bitmap_font& operator=(const bitmap_font&) = delete;

        public:
            bitmap_font(bitmap_font&&) = default;

            bitmap_font& operator=(bitmap_font&&) = default;

            bitmap_font() : _pResources(nullptr) {}

            bitmap_font(const graphics::font_info& info) noexcept;

            const graphics::font_image& getImage() const noexcept;

            const void * getTexture() const noexcept;
        };
    }
}