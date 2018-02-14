#pragma once

#include <cstddef>

#include <utility>

#include "audio/format.hpp"

namespace audio {
    class source;

    /**
     * A buffer object that transfers data to the audio device for playback.
     */
    class buffer {
        unsigned int _handle;
        bool _transient;

        friend class source;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;        

    public:
        inline buffer(buffer&& other) noexcept {
            _handle = other._handle;
            _transient = other._transient;
            
            other._handle = 0;                        
        }

        inline buffer& operator=(buffer&& other) {
            std::swap(_handle, other._handle);
            std::swap(_transient, other._transient);

            return *this;
        }

        buffer(unsigned int handle) noexcept:
            _handle(handle),
            _transient(true) {}

        buffer() noexcept:
            _handle(0),            
            _transient(false) {}        

        ~buffer() noexcept;        

        inline operator unsigned int() const noexcept {
            return _handle;
        }

        void setData(format fmt, const void * data, std::size_t size, unsigned int frequency) noexcept;
    };
}
