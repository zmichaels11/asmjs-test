#include "graphics/buffer.hpp"

#include <cstddef>
#include <iostream>

#include "graphics/buffer_info.hpp"

namespace graphics {
    const buffer& buffer::getDefault() {
        static buffer instance;

        return instance;
    }    

    const buffer_info& buffer::getInfo() const {
        return _info;
    }
}