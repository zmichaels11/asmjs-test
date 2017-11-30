#pragma once

#include "graphics/vertex_array_info.hpp"

namespace graphics {
    class buffer;

    class vertex_array {
        vertex_array_info _info;
        unsigned int _handle;

        vertex_array(const vertex_array&) = delete;

        vertex_array& operator=(const vertex_array&) = delete;

    public:
        vertex_array() : _handle(0) {}
        
        vertex_array(const vertex_array_info& info);

        ~vertex_array();

        vertex_array(vertex_array&&) = default;

        vertex_array& operator=(vertex_array&&) = default;        

        void bind() const;
    };

}
