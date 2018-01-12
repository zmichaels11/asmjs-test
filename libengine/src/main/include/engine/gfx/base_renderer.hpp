#pragma once

namespace engine {
    namespace gfx {
        class base_renderer {
        public:
            virtual void pushData(const void * pData) noexcept;

            virtual void render() const noexcept;
        };
    }
}