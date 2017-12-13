#pragma once

namespace renderer {
    class layer {
    public:
        virtual ~layer() {}

        virtual void update() = 0;

        virtual void doFrame() = 0;

        virtual void setProjection(const float * proj) = 0;
    };
}