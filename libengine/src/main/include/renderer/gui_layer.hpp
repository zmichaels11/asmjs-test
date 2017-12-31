#pragma once 

#include "renderer/layer.hpp"
#include "renderer/render_info.hpp"
#include "renderer/scissor_rect.hpp"

namespace renderer {
    class gui_layer : public virtual layer {
    public:        
        virtual ~gui_layer() {}

        virtual void update();

        virtual void render(const render_info& info);

        virtual void setProjection(const float * proj);

        virtual void setScissor(const scissor_rect& scissor);

        virtual const scissor_rect& getScissor() const;
    };
}