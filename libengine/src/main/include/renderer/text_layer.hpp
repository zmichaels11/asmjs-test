#pragma once

#include <memory>

#include "renderer/layer.hpp"
#include "renderer/text_info.hpp"
#include "renderer/text_layer_info.hpp"

namespace renderer {
    struct text_layer_res {
        virtual ~text_layer_res() {}
    };

    class text_layer : public virtual layer {
        struct scissor_t {
            float x, y, width, height;
        } scissor;

        std::shared_ptr<text_layer_res> _pResources;
        renderer::text_layer_info _info;

    public:
        text_layer(const text_layer_info& info);
        
        virtual ~text_layer() {}

        float getAscent() const;

        float getDescent() const;

        float getLineGap() const;

        inline float getVerticalSpacing() const {
            return getAscent() - getDescent() + getLineGap();
        }

        void setScissor(float x, float y, float width, float height);

        void text(const text_info& txt);

        virtual void update();

        virtual void doFrame();

        virtual void setProjection(const float * proj);

        const renderer::text_layer_info& getInfo() const;
    };
}