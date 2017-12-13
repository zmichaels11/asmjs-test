#pragma once 

#include "renderer/layer.hpp"

namespace renderer {
    class gui_layer : public virtual layer {
    public:        
        virtual ~gui_layer() {}

        virtual void update();

        virtual void doFrame();

        virtual void setProjection(const float * proj);
    };
}