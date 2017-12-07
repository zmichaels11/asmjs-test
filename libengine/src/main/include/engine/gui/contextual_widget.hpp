#pragma once

namespace engine {
    namespace gui {
        class contextual_widget {
        public:
            virtual void build(void * ctx) = 0;
        };
    }
}