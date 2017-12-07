#pragma once

#include "engine/gui/widget.hpp"

namespace engine {
    namespace gui {
        class progress_bar : public virtual widget {
            unsigned int _current, _max;

        public:
            progress_bar(unsigned int max, unsigned int current = 0) :
                _current(current),
                _max(max) {}

            virtual void build(void * ctx);

            virtual unsigned int getCurrent() const;

            virtual void setCurrent(unsigned int current);

            virtual unsigned int getMax() const;
        };
    }
}