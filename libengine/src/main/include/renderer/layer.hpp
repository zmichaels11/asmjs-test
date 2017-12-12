#pragma once

namespace renderer {
    class layer {
    public:
        virtual ~layer() {}

        virtual void onFrameStart() = 0;

        virtual void onFrameEnd() = 0;

        virtual void setProjection(const float * proj) = 0;

        virtual bool isWritable() = 0;        
    };
}