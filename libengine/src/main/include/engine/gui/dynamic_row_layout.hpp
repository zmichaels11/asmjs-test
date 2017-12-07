#pragma once

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class dynamic_row_layout : public virtual widget {
            const float _height;            
            const int _cols;
        public:
            dynamic_row_layout(float height, int cols) :
                _height(height),
                _cols(cols) {}

            virtual ~dynamic_row_layout() {}

            virtual void build(void *);

            virtual float getHeight() const;

            virtual int getColumns() const;
        };        
    }
}