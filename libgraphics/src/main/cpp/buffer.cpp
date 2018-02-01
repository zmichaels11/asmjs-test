#include "graphics/hobject/buffer.hpp"

#include "graphics/hinfo/buffer_info.hpp"

namespace graphics {
    const buffer& buffer::getDefault() noexcept {
        static buffer instance;

        return instance;
    }    

    const buffer_info& buffer::getInfo() const noexcept {
        return _info;
    }
}