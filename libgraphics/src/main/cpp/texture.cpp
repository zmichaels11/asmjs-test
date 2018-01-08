#include "graphics/hobject/texture.hpp"

#include "graphics/hinfo/texture_info.hpp"

namespace graphics {
    const texture_info& texture::getInfo() const noexcept {
        return _info;
    }
}