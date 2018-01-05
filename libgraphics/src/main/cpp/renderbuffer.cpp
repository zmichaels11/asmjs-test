#include "graphics/renderbuffer.hpp"

#include "graphics/renderbuffer_info.hpp"

namespace graphics {
    const renderbuffer_info& renderbuffer::getInfo() const noexcept {
        return _info;
    }
}