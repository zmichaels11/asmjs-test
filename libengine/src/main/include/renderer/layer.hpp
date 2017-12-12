#pragma once

namespace renderer {
    class layer {
    public:
        virtual ~layer() {}

        virtual void update(double timestep) = 0;

        virtual void doFrame(double timestep) = 0;

        virtual void setProjection(const float * proj) = 0;
    };
}