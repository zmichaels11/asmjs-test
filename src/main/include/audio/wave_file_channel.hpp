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

        bool parseSubchunk();

        void skipSubchunk(unsigned int chunkSize);

        void parseFormatSubchunk(unsigned int chunkSize);

    public:
        wave_file_channel() {}

        wave_file_channel(const std::string& path);

        wave_file_channel(const wave_file_channel&) = delete;

        wave_file_channel(wave_file_channel&&) = default;

        wave_file_channel& operator=(const wave_file_channel&) = delete;

        wave_file_channel& operator=(wave_file_channel&&) = default;

        virtual void seekStart();

        virtual void seek(unsigned int sample);

        virtual float getLength() const;

        virtual int getChannels() const;

        virtual int getSampleRate() const;

        virtual int getBitsPerSample() const;

        virtual int getByteRate() const;

        virtual format getFormat() const;

        virtual bool read(char * dst, std::size_t& n);
    };
}