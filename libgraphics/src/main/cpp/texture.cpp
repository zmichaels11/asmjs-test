#include "graphics/texture.hpp"

#include "graphics/texture_info.hpp"

namespace graphics {
    const texture_info& texture::getInfo() const {
        return _info;
    }
}