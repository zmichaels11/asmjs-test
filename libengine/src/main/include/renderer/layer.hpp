#pragma once

#include "renderer/render_info.hpp"
#include "renderer/scissor_rect.hpp"

namespace renderer {
    class layer {
    public:
        layer() {}
        
        virtual ~layer() {}

        virtual void update() {}

        virtual void render(const render_info& info) {}

        virtual void setProjection(const float * proj) {}

        virtual void setScissor(const scissor_rect& scissor) {}

        virtual const scissor_rect& getScissor() const {}
    };
}