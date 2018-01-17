#pragma once

namespace engine {
    namespace gfx {
        namespace renderer {
            struct base_renderer_resources {
                virtual ~base_renderer_resources() {}
            };

            class base_renderer {
            public:
                virtual ~base_renderer() {}

                virtual void flush() noexcept {}
            };
        }
    }
}