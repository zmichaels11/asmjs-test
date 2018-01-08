#include "graphics/hobject/shader.hpp"

#include "graphics/hinfo/shader_info.hpp"

namespace graphics {    
    const shader_info& shader::getInfo() const noexcept {
        return _info;
    }
}