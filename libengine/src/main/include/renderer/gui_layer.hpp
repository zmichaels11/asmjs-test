#pragma once 

#include "renderer/layer.hpp"

namespace renderer {
    class gui_layer : public virtual layer {
    public:        
        virtual ~gui_layer() {}

        virtual void update(double timestep);

        virtual void doFrame(double timestep);

        virtual void setProjection(const float * proj);
    };
}