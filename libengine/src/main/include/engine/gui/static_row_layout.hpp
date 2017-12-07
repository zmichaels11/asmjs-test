#pragma once

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class static_row_layout : public virtual widget {
            int _height;
            int _cols;
            int _itemWidth;
        
        public:
            static_row_layout(int height, int itemWidth, int cols) :
                _height(height),
                _itemWidth(itemWidth),
                _cols(cols) {}

            virtual ~static_row_layout() {}

            virtual void build(void *);

            virtual int getHeight() const;

            virtual int getColumns() const;

            virtual int getItemWidth() const;
        };        
    }
}