#pragma once

#include "engine/gui/component.hpp"

namespace engine {
    namespace gui {
        class static_row_layout : public virtual component {
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

            inline int getHeight() const;

            inline int getColumns() const;

            inline int getItemWidth() const;
        };

        int static_row_layout::getHeight() const {
            return _height;
        }

        int static_row_layout::getItemWidth() const {
            return _itemWidth;
        }

        int static_row_layout::getColumns() const {
            return _cols;
        }
    }
}