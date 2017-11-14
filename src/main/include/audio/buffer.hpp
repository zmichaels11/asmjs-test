#pragma once

#include <AL/al.h>

#include "audio/format.hpp"

namespace audio {
    class source;

    class buffer {
        ALuint _handle;
        bool _transient;
        friend class source;

        buffer(const buffer&) = delete;

        buffer& operator=(const buffer&) = delete;

    public:
        buffer();

        buffer(ALuint handle) : 
            _handle(handle), 
            _transient(true) {}        

        buffer(buffer&&) = default;

        virtual ~buffer();        

        buffer& operator=(buffer&&) = default;

        void setData(format fmt, const void * data, int size, int freq) const;
    };
}