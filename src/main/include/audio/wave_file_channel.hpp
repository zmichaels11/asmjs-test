#pragma once

#include <cstdio>
#include <string>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"

namespace audio {
    class wave_file_channel : public sound_channel {
        FILE * _pFile;
        long _offset;
        int _channels;
        int _sampleRate;
        int _bitsPerSample;
        int _byteRate;
        int _size;
        long _dataStart;
        format _format;

        bool parseSubchunk();

        void skipSubchunk(unsigned int chunkSize);

        void parseFormatSubchunk(unsigned int chunkSize);

    public:
        wave_file_channel(const std::string& path);

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

        void close();
    };
}