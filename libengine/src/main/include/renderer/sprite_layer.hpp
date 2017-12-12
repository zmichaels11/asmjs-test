#pragma once

#include <cstddef>

#include "renderer/image_view.hpp"
#include "renderer/layer.hpp"

namespace renderer {
    class sprite_layer : public virtual renderer::layer {
    public:
        virtual ~sprite_layer() {}

        virtual const renderer::sprite_layer_info& getInfo() const = 0;

        virtual const renderer::image_view& getSpriteView(int spriteId) const = 0;

        virtual const renderer::image_view& getMaskView(int maskId) const = 0;

        virtual void clear() = 0;

        virtual bool submit(const renderer::sprite_info * pSprites, std::size_t count) = 0;
    };
}