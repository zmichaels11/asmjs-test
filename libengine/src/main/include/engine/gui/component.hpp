#pragma once

namespace engine {
    namespace gui {
        class component {
        public:                        
            virtual void build(void * ctx) = 0;
        };
    }
}