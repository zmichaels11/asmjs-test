#pragma once

#include <cstddef>

#include "audio/format.hpp"

namespace audio {
    class sound_channel {
    public:
        virtual ~sound_channel() noexcept {}

        virtual int getChannels() const noexcept = 0;
        virtual int getSampleRate() const noexcept = 0;
        virtual int getBitsPerSample() const noexcept = 0;
        
        virtual int getByteRate() const noexcept = 0;
        virtual format getFormat() const noexcept = 0;
        virtual void seekStart() noexcept = 0;
        virtual void seek(unsigned int sample) noexcept = 0;
        virtual float getLength() const noexcept = 0;

        virtual bool read(void * dst, std::size_t& n) noexcept = 0;
    };
}