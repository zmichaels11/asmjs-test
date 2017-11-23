#pragma once

namespace graphics {
    class buffer;

    struct vertex_binding_description {
        unsigned int binding;
        unsigned int stride;
        unsigned int divisor;
        graphics::buffer * buffer;
        long offset;
    };
}