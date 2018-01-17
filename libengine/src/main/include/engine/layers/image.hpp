#pragma once

#include <cstddef>

namespace engine {
    namespace layers {
        class image {
        public:
            virtual ~image() {}
            
            virtual const void * getData() const = 0;

            virtual std::size_t getWidth() const = 0;

            virtual std::size_t getHeight() const = 0;
        };
    }
}