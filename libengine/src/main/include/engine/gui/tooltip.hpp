#pragma once

#include <string>

namespace engine {
    namespace gui {
        class tooltip {
            const std::string _tip;
            const float _width;

        public:
            tooltip(const std::string& tip, float width) :
                _tip(tip),
                _width(width) {}

            virtual ~tooltip() {}

            virtual void build(void * ctx);

            virtual const std::string& getTip() const;

            virtual float getWidth() const;
        };
    }
}