#pragma once

#include <cstddef>
#include <string>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"
#include "audio/stb_vorbis.hpp"

namespace audio {
    class vorbis_file_channel : public sound_channel {
        
        int _channels;
        int _sampleRate;
        int _byteRate;
        int _size;
        long _dataStart;
        float _time;
        format _format;

        stb_vorbis * _handle;

    public:
        vorbis_file_channel() {}

        vorbis_file_channel(const std::string& path);

        vorbis_file_channel(const vorbis_file_channel&) = delete;

        vorbis_file_channel(vorbis_file_channel&&) = default;

        virtual ~vorbis_file_channel();

        vorbis_file_channel& operator=(const vorbis_file_channel&) = delete;

        vorbis_file_channel& operator=(vorbis_file_channel&&) = default;

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