#pragma once

#include <cstddef>

#include <string>

#include "graphics/henum/buffer_target.hpp"
#include "graphics/hinfo/buffer_info.hpp"

namespace graphics {
    class vertex_array;
    
    class buffer {
        unsigned int _handle;
        buffer_info _info;
        bool _external;
        std::string _name;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;

        friend class vertex_array;
    public:
        buffer(buffer&&) = default;

        buffer& operator=(buffer&&) = default;        

        buffer() noexcept: 
            _handle(0),
            _info(),
            _external(false),
            _name("0") {}
        
        buffer(const buffer_info& info) noexcept;

        buffer(unsigned int handle) noexcept:
            _handle(handle),
            _info(),
            _external(true),
            _name(std::to_string(handle)) {}

        ~buffer() noexcept;

        void bind(buffer_target target) const noexcept;

        inline void bind() const noexcept {
            bind(_info.target);
        }

        void invalidate() const noexcept;

        void subData(long offset, const void * src, std::size_t size) const noexcept;

        void bindBase(unsigned int index) const noexcept;

        void bindRange(unsigned int index, long offset, std::size_t size) const noexcept;

        const graphics::buffer_info& getInfo() const noexcept;

        inline operator int() const noexcept {
            return _handle;
        }

        static const buffer& getDefault() noexcept;

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;
    };
}