#include "graphics/hobject/texture.hpp"

#include "graphics/hinfo/texture_info.hpp"
#include "graphics/hobject/image.hpp"

namespace graphics {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    const texture_info& texture::getInfo() const noexcept {
        return _info;
    }

    void texture::subImage(
        unsigned int level, 
        int x, int y, int z, 
        const image * pImage) const noexcept {

        subImage(level, x, y, z, pImage->getWidth(), pImage->getHeight(), 1, {
            pImage->getType(),
            pImage->getFormat(),
            const_cast<void *> (pImage->getData())});
    }
}
