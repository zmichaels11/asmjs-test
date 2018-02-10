#pragma once

#include <cstddef>

#include "audio/buffer_info.hpp"
#include "audio/format.hpp"

namespace audio {
    class source;

    /**
     * A buffer object that transfers data to the audio device for playback.
     */
    class buffer {
        unsigned int _handle;
        buffer_info _info;

        friend class source;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;

    public:
        buffer(buffer&&) = default;

        buffer& operator=(buffer&&) = default;

        buffer() noexcept:
            _handle(0),            
            _info() {}
        
        buffer(const buffer_info& info) noexcept;        

        ~buffer() noexcept;

        const buffer_info& getInfo() const noexcept;

        inline operator unsigned int() noexcept {
            return _handle;
        }
    };
}
