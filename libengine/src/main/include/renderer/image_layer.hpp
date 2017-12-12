#pragma once

#include "renderer/color_transform.hpp"
#include "renderer/layer.hpp"

namespace renderer {
    class image_layer : public virtual renderer::layer {
    public:
        virtual ~image_layer() {}

        virtual void setColorTransform(const renderer::color_transform& ct) = 0;

        virtual void scrollImage(float s0, float t0, float s1, float t1) = 0;

        virtual void scrollMask(float s0, float t0, float s1, float t1) = 0;
    };
}