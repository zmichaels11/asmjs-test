#include "graphics/program.hpp"

#include "graphics/program_info.hpp"

namespace graphics {   
    const program_info& program::getInfo() const {
        return _info;
    }
}