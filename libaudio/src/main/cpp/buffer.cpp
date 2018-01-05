#include "audio/buffer.hpp"

#include <AL/al.h>

#include "audio/format.hpp"

namespace audio {
    buffer::buffer() noexcept {
        alGenBuffers(1, &_handle);
        _transient = false;
    }    

    buffer::~buffer() noexcept {
        if (_handle && !_transient) {
            alDeleteBuffers(1, &_handle);
            _handle = 0;
        }
    }

    void buffer::setData(format fmt, const void * data, std::size_t size, unsigned int freq) const noexcept {
        alBufferData(_handle, static_cast<ALenum> (fmt), data, size, freq);
    }
}