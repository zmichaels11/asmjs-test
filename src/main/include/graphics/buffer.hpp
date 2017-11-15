#pragma once

#include <GLES3/gl3.h>

#include <cstddef>

#include "graphics/buffer_info.hpp"
#include "graphics/buffer_target.hpp"

namespace graphics {
    class vertex_array;

    class buffer {
        GLuint _handle;
        buffer_info _info;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;

        friend class vertex_array;
    public:
        buffer(const buffer_info& info);

        ~buffer();

        buffer(buffer&&) = default;

        buffer& operator=(buffer&&) = default;

        inline void bind() const {
            bind(_info.target);
        }

        void bind(buffer_target target) const;        

        void subData(long offset, const void * src, std::size_t size) const;

        void bindBase(unsigned int index) const;

        void bindRange(unsigned int index, long offset, std::size_t size) const;

        const buffer_info& getInfo() const;
    };
}