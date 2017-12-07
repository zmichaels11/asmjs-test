#pragma once

namespace engine {
    namespace gui {
        class widget {
        public:
            virtual void build(void *) = 0;
        };
    }
}