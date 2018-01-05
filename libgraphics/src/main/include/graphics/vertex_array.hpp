#pragma once

#include "graphics/buffer.hpp"
#include "graphics/vertex_array_info.hpp"

namespace graphics {
    class vertex_array {
        vertex_array_info _info;
        unsigned int _handle;
        bool _external;

        vertex_array(const vertex_array&) = delete;

        vertex_array& operator=(const vertex_array&) = delete;

    public:
        vertex_array(vertex_array&&) = default;

        vertex_array& operator=(vertex_array&&) = default;        

        vertex_array() noexcept:
            _info(), 
            _handle(0),
            _external(false) {}
        
        vertex_array(const vertex_array_info& info) noexcept;

        vertex_array(unsigned int handle)  noexcept:
            _info(),
            _handle(handle),
            _external(true) {}

        ~vertex_array() noexcept;

        void bind() const noexcept;

        inline operator int() const noexcept {
            return _handle;
        }

        const graphics::vertex_array_info& getInfo() const noexcept;

        static const vertex_array& getDefault() noexcept;
    };

}
