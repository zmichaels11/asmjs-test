#pragma once

#include <cstddef>

#include "graphics/buffer_info.hpp"
#include "graphics/buffer_target.hpp"
#include "graphics/vertex_array.hpp"

namespace graphics {
    class buffer {
        unsigned int _handle;
        buffer_info _info;
        bool _external;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;

        friend class vertex_array;
    public:
        buffer() : 
            _handle(0),
            _info(),
            _external(false) {}
        
        buffer(const buffer_info& info);

        buffer(unsigned int handle) :
            _handle(handle),
            _info(),
            _external(true) {}

        ~buffer();

        buffer(buffer&&) = default;

        buffer& operator=(buffer&&) = default;        

        void bind(buffer_target target) const;

        inline void bind() const {
            bind(_info.target);
        }

        void invalidate() const;

        void subData(long offset, const void * src, std::size_t size) const;

        void bindBase(unsigned int index) const;

        void bindRange(unsigned int index, long offset, std::size_t size) const;

        const graphics::buffer_info& getInfo() const;

        inline operator int() const {
            return _handle;
        }

        static const buffer& getDefault();
    };
}