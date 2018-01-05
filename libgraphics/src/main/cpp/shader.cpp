#include "graphics/shader.hpp"

#include "graphics/shader_info.hpp"

namespace graphics {    
    const shader_info& shader::getInfo() const noexcept {
        return _info;
    }
}