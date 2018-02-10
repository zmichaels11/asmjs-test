#include "pch.h"
#include "audio/buffer.hpp"

#include "audio/format.hpp"

namespace audio {
    buffer::~buffer() noexcept {        
        if (!_transient && _handle) {            
            alDeleteBuffers(1, &_handle);
        }
    }

    void buffer::setData(format fmt, const void * data, std::size_t size, unsigned int frequency) noexcept {
        if (_handle == 0) {
            alGenBuffers(1, &_handle);
        }

        alBufferData(
            _handle, 
            static_cast<ALenum> (fmt),
            data, size,
            static_cast<ALsizei> (frequency));
    }


}
