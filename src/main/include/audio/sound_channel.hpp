#pragma once

#include "audio/format.hpp"

namespace audio {
    class sound_channel {
    public:
        
        virtual int getChannels() const = 0;
        virtual int getSampleRate() const = 0;
        virtual int getBitsPerSample() const = 0;
        
        virtual int getByteRate() const = 0;
        virtual format getFormat() const = 0;
        virtual void seekStart() = 0;
        virtual void seek(unsigned int sample) = 0;
        virtual float getLength() const = 0;

        virtual int read(char * dst, unsigned int n) = 0;
        virtual bool isOpen() const = 0;
        virtual void close() = 0;
    };
}