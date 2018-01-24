#pragma once

namespace engine {
    namespace layers {
        class renderable {
        public:
            virtual ~renderable() {}

            virtual void beginWrite() noexcept = 0;

            virtual void endWrite() noexcept = 0;

            virtual void render() const noexcept = 0;

            virtual void invalidate() noexcept = 0;

            virtual const void * getTexture() const noexcept = 0;
        };
    }
}