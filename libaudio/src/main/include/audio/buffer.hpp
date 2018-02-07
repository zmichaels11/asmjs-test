#pragma once

#include <cstddef>

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

        buffer(unsigned int handle) noexcept: 
            _handle(handle), 
            _transient(true) {}

    public:
        buffer(buffer&&) = default;

        buffer& operator=(buffer&&) = default;

        /**
         * Constructs a new buffer object.
         */
        buffer() noexcept;        

        virtual ~buffer() noexcept;        

        /**
         * Sets the contents of the audio buffer.
         * 
         * @param fmt the audio format.
         * @param data the data to write
         * @param size the size of the data in basic machine units
         * @param freq the audio frequency in hz
         */
        void setData(format fmt, const void * data, std::size_t size, unsigned int freq) const noexcept;
    };
}
