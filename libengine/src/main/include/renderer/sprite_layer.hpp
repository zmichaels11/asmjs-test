#pragma once

#include <cstddef>

#include "renderer/image_view.hpp"
#include "renderer/layer.hpp"
#include "renderer/sprite_info.hpp"
#include "renderer/sprite_layer_info.hpp"

namespace renderer {
    class sprite_layer : public virtual renderer::layer {
    public:
        virtual ~sprite_layer() {}

        virtual const renderer::sprite_layer_info& getInfo() const = 0;

        virtual void * getSpriteView(int spriteId) const = 0;

        virtual void * getMaskView(int maskId) const = 0;

        virtual void clear() = 0;

        virtual bool submit(const renderer::sprite_info * pSprites, std::size_t count) = 0;
    };
}