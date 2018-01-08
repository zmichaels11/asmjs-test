#pragma once

#include "graphics/henum/vertex_format.hpp"

namespace graphics {
    struct vertex_attribute_description {
        unsigned int location;
        graphics::vertex_format format;
        long offset;
        unsigned int binding;
    };
}