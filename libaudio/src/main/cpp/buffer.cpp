#include "audio/buffer.hpp"

#include <AL/al.h>
#include <cstdio>

#include "audio/format.hpp"

namespace audio {
    buffer::buffer() {
        alGenBuffers(1, &_handle);
        _transient = false;
    }    

    buffer::~buffer() {
        if (_handle && !_transient) {
            alDeleteBuffers(1, &_handle);
            _handle = 0;
        }
    }

    void buffer::setData(format fmt, const void * data, int size, int freq) const {
        alBufferData(_handle, static_cast<ALenum> (fmt), data, size, freq);
    }
}