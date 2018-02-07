#include "graphics/hobject/vertex_array.hpp"

#include "graphics/hinfo/vertex_array_info.hpp"

namespace graphics {    
    const vertex_array& vertex_array::getDefault() noexcept {
        static vertex_array instance;

        return instance;
    }

    const graphics::vertex_array_info& vertex_array::getInfo() const noexcept {
        return _info;
    }
}
