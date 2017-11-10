#pragma once

#include <cstdio>
#include <cstdint>
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
        long _offset;
        long _dataStart;
        format _format;
        FILE * _pFile;

        bool parseSubchunk();

        void skipSubchunk(unsigned int chunkSize);

        void parseFormatSubchunk(unsigned int chunkSize);

    public:
        wave_file_channel(const std::string& path);

        virtual ~wave_file_channel();

        void seekStart();

        void seek(unsigned int sample);

        float getLength() const;

        int getChannels() const;

        int getSampleRate() const;

        int getBitsPerSample() const;

        int getByteRate() const;

        format getFormat() const;

        int read(char * dst, unsigned int n);

        bool isOpen() const;
    };
}