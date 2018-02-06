#pragma once

#include <string>

#include "graphics/hinfo/vertex_array_info.hpp"

namespace graphics { 
    class command_buffer;

    class vertex_array {
        friend class command_buffer;
        
        vertex_array_info _info;
        unsigned int _handle;
        bool _external;
        std::string _name;

        vertex_array(const vertex_array&) = delete;

        vertex_array& operator=(const vertex_array&) = delete;

    public:
        vertex_array(vertex_array&&) = default;

        vertex_array& operator=(vertex_array&&) = default;        

        vertex_array() noexcept:
            _info(), 
            _handle(0),
            _external(false),
            _name("0") {}
        
        vertex_array(const vertex_array_info& info) noexcept;

        vertex_array(unsigned int handle)  noexcept:
            _info(),
            _handle(handle),
            _external(true),
            _name(std::to_string(handle)) {}

        ~vertex_array() noexcept;

        void bind() const noexcept;

        inline operator unsigned int() const noexcept {
            return _handle;
        }

        const graphics::vertex_array_info& getInfo() const noexcept;

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;

        static const vertex_array& getDefault() noexcept;

        inline static vertex_array empty() noexcept {
            return vertex_array({nullptr});
        }
    };

}
