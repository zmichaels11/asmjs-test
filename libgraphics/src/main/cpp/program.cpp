#include "graphics/hobject/program.hpp"

#include "graphics/hinfo/program_info.hpp"

namespace graphics {   
    const program_info& program::getInfo() const noexcept {
        return _info;
    }
}
