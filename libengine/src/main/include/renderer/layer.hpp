#pragma once

#include "renderer/render_info.hpp"
#include "renderer/scissor_rect.hpp"

namespace renderer {
    class layer {
    public:
        virtual ~layer() {}

        virtual void update() = 0;

        virtual void render(const render_info& info) = 0;

        virtual void setProjection(const float * proj) = 0;

        virtual void setScissor(const scissor_rect& scissor) = 0;

        virtual const scissor_rect& getScissor() const = 0;
    };
}