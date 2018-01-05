#pragma once

#include <cstddef>

#include <memory>

#include "renderer/image_view.hpp"
#include "renderer/layer.hpp"
#include "renderer/render_info.hpp"
#include "renderer/scissor_rect.hpp"
#include "renderer/sprite_info.hpp"
#include "renderer/sprite_layer_info.hpp"

namespace renderer {
    struct sprite_layer_res {
        virtual ~sprite_layer_res(){}
    };
    
    class sprite_layer : public virtual renderer::layer {
        scissor_rect _scissor;
        std::unique_ptr<sprite_layer_res> _pResources;
        renderer::sprite_layer_info _info;
        
    public:
        sprite_layer(const sprite_layer_info& info);

        virtual ~sprite_layer() {}

        virtual void update();

        virtual void render(const render_info& info);

        virtual void setProjection(const float* proj);

        virtual void setScissor(const scissor_rect& scissor);

        virtual const scissor_rect& getScissor() const;

        virtual const renderer::sprite_layer_info& getInfo() const;

        virtual const image_view& getSpriteView(int spriteId) const;

        virtual bool submit(const sprite_info * pSprites, std::size_t count = 1);
    };
}