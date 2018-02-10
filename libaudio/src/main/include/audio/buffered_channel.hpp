#pragma once

#include <cstddef>

#include <list>
#include <vector>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"

namespace audio {
    class buffered_channel : public sound_channel {
        std::list<std::vector<char>> _buffers;
        format _format;
        int _sampleRate;
        int _channels;
        int _bitsPerSample;
        int _byteRate;
        float _length;
        int _currentBuffer;
        int _currentOffset;
        unsigned int _bufferSize;

    public:
        buffered_channel(sound_channel&& base) noexcept;

        buffered_channel(const buffered_channel& other) noexcept;

        virtual ~buffered_channel() {}

        virtual int getChannels() const noexcept;

        virtual int getSampleRate() const noexcept;

        virtual int getBitsPerSample() const noexcept;

        virtual int getByteRate() const noexcept;

        virtual format getFormat() const noexcept;

        virtual void seekStart() noexcept;

        virtual void seek(unsigned int sample) noexcept;

        virtual float getLength() const noexcept;

        virtual bool read(void * dst, std::size_t& n) noexcept;
    };
}
