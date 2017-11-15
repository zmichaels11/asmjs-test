#pragma once

#include <GLES3/gl3.h>

#include "graphics/vertex_array_info.hpp"

namespace graphics {
    class buffer;

    class vertex_array {
        vertex_array_info _info;
        GLuint _handle;

        vertex_array(const vertex_array&) = delete;

        vertex_array& operator=(const vertex_array&) = delete;

    public:
        vertex_array(const vertex_array_info& info);

        ~vertex_array();

        vertex_array(vertex_array&&) = default;

        vertex_array& operator=(vertex_array&&) = default;        

        void bind() const;
    };

}
