#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"

namespace audio {
    class wave_file_channel : public sound_channel {        

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

        wave_file_channel(const wave_file_channel&) = delete;
        
        wave_file_channel& operator=(const wave_file_channel&) = delete;
        
    public:        
        wave_file_channel& operator=(wave_file_channel&&) = default;

        wave_file_channel(wave_file_channel&&) = default;                

        wave_file_channel() noexcept {}

        wave_file_channel(const std::string& path) noexcept;

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
