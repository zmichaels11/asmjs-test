#pragma once

#include <cstddef>

#include <string>
#include <utility>

#include "graphics/hbitfield/buffer_access.hpp"
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
        inline buffer(buffer&& other) noexcept {
            _handle = other._handle;
            _info = std::move(_info);
            _external = other._external;
            _name = std::move(_name);

            other._handle = 0;
        }

        inline buffer& operator=(buffer&& other) {
            std::swap(_handle, other._handle);
            std::swap(_info, other._info);
            std::swap(_external, other._external);
            std::swap(_name, other._name);
            
            return *this;
        }

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

        void * map(long offset, std::size_t size, buffer_access access) const noexcept;

        void unmap() const noexcept;

        void invalidate() const noexcept;

        void subData(long offset, const void * src, std::size_t size) const noexcept;

        void bindBase(unsigned int index) const noexcept;

        void bindRange(unsigned int index, long offset, std::size_t size) const noexcept;

        const graphics::buffer_info& getInfo() const noexcept;

        inline operator unsigned int() const noexcept {
            return _handle;
        }

        static const buffer& getDefault() noexcept;

        void setName(const std::string& name) noexcept;

        const std::string& getName() const noexcept;
    };
}