#include "pch.h"
#include "audio/buffer.hpp"

#include "audio/format.hpp"

namespace audio {
    buffer::buffer(const buffer_info& info) noexcept {
        _info = info;

        alGenBuffers(1, &_handle);        
        alBufferData(_handle, static_cast<ALenum> (info.format), info.data, static_cast<ALsizei> (info.size), static_cast<ALsizei> (info.frequency));
    }    

    buffer::~buffer() noexcept {        
        if (_handle) {
            alDeleteBuffers(1, &_handle);
        }
    }

    const buffer_info& buffer::getInfo() const noexcept {
        return _info;
    }


}
