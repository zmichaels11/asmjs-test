#pragma once

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class dynamic_row_layout : public virtual component {
            float _height;            
            int _cols;
        public:
            dynamic_row_layout(float height, int cols) :
                _height(height),
                _cols(cols) {}

            virtual ~dynamic_row_layout() {}

            virtual void build();

            inline float getHeight() const;

            inline int getColumns() const;
        };

        float dynamic_row_layout::getHeight() const {
            return _height;
        }

        int dynamic_row_layout::getColumns() const {
            return _cols;
        }
    }
}