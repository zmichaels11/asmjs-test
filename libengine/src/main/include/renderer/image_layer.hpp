#pragma once

#include <memory>

#include "renderer/color_transform.hpp"
#include "renderer/image_layer_info.hpp"
#include "renderer/layer.hpp"
#include "renderer/render_info.hpp"
#include "renderer/scissor_rect.hpp"

namespace renderer {
    struct image_layer_res {
        virtual ~image_layer_res() {}
    };

    class image_layer : public virtual renderer::layer {
        scissor_rect _scissor;
        image_layer_info _info;
        std::unique_ptr<image_layer_res> _pResources;

    public:
        image_layer(const image_layer_info& info);

        virtual ~image_layer() {}

        virtual void update();

        virtual void render(const render_info& info);

        virtual void setProjection(const float * projection);

        virtual void setScissor(const scissor_rect& scissor);

        virtual const scissor_rect& getScissor() const;

        void setColorTransform(const renderer::color_transform& ct);

        void scrollImage(float s0, float t0, float s1, float t1);

        void scrollMask(float s0, float t0, float s1, float t1);

        const image_layer_info& getInfo() const;        
    };
}