#include "graphics/hobject/framebuffer.hpp"

namespace graphics {
    const framebuffer& framebuffer::getDefault() noexcept {
        static framebuffer INSTANCE;

        return INSTANCE;
    }
}
