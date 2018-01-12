#pragma once

namespace engine {
    namespace gfx {
        class base_renderer {
        public:
            virtual void pushData(const void * pData) noexcept = 0;

            virtual void reset() noexcept = 0;

            virtual void update() noexcept = 0;

            virtual void render() const noexcept = 0;
        };
    }
}