#pragma once

#include <cstddef>

#include "audio/format.hpp"

namespace audio {
    class sound_channel {
    public:
        virtual ~sound_channel(){}

        virtual int getChannels() const = 0;
        virtual int getSampleRate() const = 0;
        virtual int getBitsPerSample() const = 0;
        
        virtual int getByteRate() const = 0;
        virtual format getFormat() const = 0;
        virtual void seekStart() = 0;
        virtual void seek(unsigned int sample) = 0;
        virtual float getLength() const = 0;

        virtual bool read(char * dst, std::size_t& n) = 0;
    };
}