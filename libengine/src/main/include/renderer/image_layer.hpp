#pragma once

#include <memory>

#include "renderer/color_transform.hpp"
#include "renderer/image_layer_info.hpp"
#include "renderer/layer.hpp"

namespace renderer {
    struct image_layer_res {
        virtual ~image_layer_res() {}
    };

    class image_layer : public virtual renderer::layer {
        image_layer_info _info;
        std::shared_ptr<image_layer_res> _pResources;

    public:
        image_layer(const image_layer_info& info);

        virtual ~image_layer() {}

        virtual void update();

        virtual void doFrame();

        virtual void setProjection(const float * projection);

        void setColorTransform(const renderer::color_transform& ct);

        void scrollImage(float s0, float t0, float s1, float t1);

        void scrollMask(float s0, float t0, float s1, float t1);

        const image_layer_info& getInfo() const;
    };
}