#include "graphics/hobject/renderbuffer.hpp"

#include "graphics/hinfo/renderbuffer_info.hpp"

namespace graphics {
    const renderbuffer_info& renderbuffer::getInfo() const noexcept {
        return _info;
    }
}