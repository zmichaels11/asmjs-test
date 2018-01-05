#pragma once

#include <cstddef>
#include <memory>

#include "renderer/layer.hpp"
#include "renderer/render_info.hpp"
#include "renderer/scissor_rect.hpp"
#include "renderer/text_info.hpp"
#include "renderer/text_layer_info.hpp"

namespace renderer {
    struct text_layer_res {
        virtual ~text_layer_res() {}
    };

    class text_layer : public virtual layer {
        scissor_rect _scissor;

        std::shared_ptr<text_layer_res> _pResources;
        renderer::text_layer_info _info;

    public:
        text_layer(const text_layer_info& info);

        const text_layer_info& getInfo() const;
        
        virtual ~text_layer() {}

        float getAscent() const;

        float getDescent() const;

        float getLineGap() const;

        inline float getVerticalSpacing() const {
            return getAscent() - getDescent() + getLineGap();
        }

        virtual void setScissor(const scissor_rect& scissor);

        virtual const scissor_rect& getScissor() const;

        void submit(const text_info * pTexts, std::size_t count = 1);

        virtual void update();

        virtual void render(const render_info& info);

        virtual void setProjection(const float * proj);
    };
}