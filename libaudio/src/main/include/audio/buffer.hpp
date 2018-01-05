#pragma once

#include <cstddef>

#include "audio/format.hpp"

namespace audio {
    class source;

    class buffer {
        unsigned int _handle;
        bool _transient;
        friend class source;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;

    public:
        buffer(buffer&&) noexcept = default;

        buffer& operator=(buffer&&) noexcept = default;

        buffer() noexcept;

        buffer(unsigned int handle) noexcept: 
            _handle(handle), 
            _transient(true) {}                

        virtual ~buffer() noexcept;        

        void setData(format fmt, const void * data, std::size_t size, unsigned int freq) const noexcept;
    };
}