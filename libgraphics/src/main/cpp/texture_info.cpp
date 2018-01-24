#include "graphics/hinfo/texture_info.hpp"

#include "graphics/henum/internal_format.hpp"
#include "graphics/hinfo/sampler_info.hpp"
#include "graphics/hobject/image.hpp"

namespace graphics {
    texture_info texture_info::ofImage(const image * pImage) noexcept {        
        return {
            {pImage->getWidth(), pImage->getHeight(), 1},
            1, 1,
            sampler_info::defaults(),
            join(pImage->getType(), pImage->getFormat())};
    }
}