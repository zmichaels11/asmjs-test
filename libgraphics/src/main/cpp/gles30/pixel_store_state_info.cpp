#if defined(GLES30)

#include "graphics/hstate/pixel_store_state_info.hpp"

#include <GLES3/gl3.h>

namespace graphics {
    template<>
    void apply(const pixel_store_state_info& info) noexcept {
//        glPixelStorei(GL_PACK_SWAP_BYTES, info.pack.swapBytes);
//        glPixelStorei(GL_PACK_LSB_FIRST, info.pack.lsbFirst);
        glPixelStorei(GL_PACK_ROW_LENGTH, info.pack.rowLength);
//        glPixelStorei(GL_PACK_IMAGE_HEIGHT, info.pack.imageHeight);
        glPixelStorei(GL_PACK_SKIP_ROWS, info.pack.skipRows);
        glPixelStorei(GL_PACK_SKIP_PIXELS, info.pack.skipPixels);
//        glPixelStorei(GL_PACK_SKIP_IMAGES, info.pack.skipImages);
        glPixelStorei(GL_PACK_ALIGNMENT, info.pack.alignment);

        glPixelStorei(GL_UNPACK_ROW_LENGTH, info.unpack.rowLength);
//        glPixelStorei(GL_UNPACK_LSB_FIRST, info.unpack.lsbFirst);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, info.unpack.rowLength);
        glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, info.unpack.imageHeight);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, info.unpack.skipRows);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, info.unpack.skipPixels);
        glPixelStorei(GL_UNPACK_SKIP_IMAGES, info.unpack.skipImages);
//        glPixelStorei(GL_UNPACK_SKIP_ALIGNMENT, info.unpack.alignment);
    }
}
#endif