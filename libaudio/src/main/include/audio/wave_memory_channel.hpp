#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"

namespace audio {
    class wave_memory_channel : public sound_channel {        
        const char * _data;
        long _offset;
        std::size_t _len;
        std::int16_t _channels;
        std::int16_t _bitsPerSample;
        std::int32_t _sampleRate;
        std::int32_t _byteRate;
        std::int32_t _size;
        long _dataStart;
        format _format;
        std::ifstream _file;

        bool parseSubchunk() noexcept;

        void skipSubchunk(unsigned int chunkSize) noexcept;

        void parseFormatSubchunk(unsigned int chunkSize) noexcept;        

        wave_memory_channel(const wave_memory_channel&) = delete;
        
        wave_memory_channel& operator=(const wave_memory_channel&) = delete;
        
    public:        
        wave_memory_channel& operator=(wave_memory_channel&&) = default;

        wave_memory_channel(wave_memory_channel&&) = default;                

        wave_memory_channel() noexcept {}

        wave_memory_channel(const char * data, std::size_t len) noexcept;

        virtual void seekStart() noexcept;

        virtual void seek(unsigned int sample) noexcept;

        virtual float getLength() const noexcept;

        virtual int getChannels() const noexcept;

        virtual int getSampleRate() const noexcept;

        virtual int getBitsPerSample() const noexcept;

        virtual int getByteRate() const noexcept;

        virtual format getFormat() const noexcept;

        virtual bool read(void * dst, std::size_t& n) noexcept;
    };
}
