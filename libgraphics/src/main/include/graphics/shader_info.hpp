#pragma once

#include "graphics/shader_type.hpp"

#include <string>

namespace graphics {
    struct shader_info {
        graphics::shader_type type;
        std::string src;
    };
}