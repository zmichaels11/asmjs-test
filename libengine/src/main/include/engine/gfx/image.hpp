#pragma once

namespace engine {
    namespace gfx {
        class image {
        public:
            virtual ~image();

            virtual unsigned int getWidth() const noexcept;

            virtual unsigned int getHeight() const noexcept;
        };
    }
}