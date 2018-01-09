#pragma once

namespace renderer {
    namespace tree {
        class node {
        public:
            virtual ~node() noexcept {};

            virtual void setProjection(const float * proj) noexcept;
        };
    }    
}