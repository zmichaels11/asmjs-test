#pragma once

#include <cstddef>
#include <string>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"

namespace audio {
    class vorbis_file_channel : public sound_channel {
        
        int _channels;
        int _sampleRate;
        int _byteRate;
        int _size;
        long _dataStart;
        float _time;
        format _format;

        void * _handle;

        vorbis_file_channel(vorbis_file_channel&) = delete;

        vorbis_file_channel& operator=(const vorbis_file_channel&) = delete;

    public:
        vorbis_file_channel() :
            _channels(0),
            _sampleRate(0),
            _byteRate(0),
            _size(0),
            _dataStart(0),
            _time(0),
            _format(static_cast<format>(0)),
            _handle(nullptr){}

        vorbis_file_channel(const std::string& path);

        virtual ~vorbis_file_channel();        

        vorbis_file_channel(vorbis_file_channel&&) = default;                

        vorbis_file_channel& operator=(vorbis_file_channel&&) = default;

        virtual void seekStart();

        virtual void seek(unsigned int sample);

        virtual float getLength() const;

        virtual int getChannels() const;

        virtual int getSampleRate() const;

        virtual int getBitsPerSample() const;

        virtual int getByteRate() const;

        virtual format getFormat() const;

        virtual bool read(float * dst, std::size_t& n);

        virtual bool read(char * dst, std::size_t& n);
    };
}