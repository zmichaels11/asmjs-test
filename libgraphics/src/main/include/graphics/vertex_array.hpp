#pragma once

#include "graphics/buffer.hpp"
#include "graphics/vertex_array_info.hpp"

namespace graphics {
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

        inline operator int() const {
            return _handle;
        }

        const graphics::vertex_array_info& getInfo() const;

        static const vertex_array& getDefault();
    };

}
