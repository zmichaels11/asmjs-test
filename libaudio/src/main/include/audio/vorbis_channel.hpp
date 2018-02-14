#pragma once

#include <cstddef>
#include <string>
#include <utility>

#include "audio/format.hpp"
#include "audio/sound_channel.hpp"

namespace audio {
    class vorbis_channel : public sound_channel {
        
        int _channels;
        int _sampleRate;
        int _byteRate;
        int _size;
        long _dataStart;
        float _time;
        format _format;

        void * _handle;

        vorbis_channel(vorbis_channel&) = delete;

        vorbis_channel& operator=(const vorbis_channel&) = delete;

    public:
        inline vorbis_channel(vorbis_channel&& other) noexcept {
            _channels = other._channels;
            _sampleRate = other._sampleRate;
            _byteRate = other._byteRate;
            _size = other._size;
            _dataStart = other._dataStart;
            _time = other._time;
            _format = other._format;
            _handle = other._handle;

            other._handle = nullptr;            
        }

        inline vorbis_channel& operator=(vorbis_channel&& other) {
            std::swap(_channels, other._channels);
            std::swap(_sampleRate, other._sampleRate);
            std::swap(_byteRate, other._byteRate);
            std::swap(_size, other._size);
            std::swap(_dataStart, other._dataStart);
            std::swap(_time, other._time);
            std::swap(_format, other._format);
            std::swap(_handle, other._handle);

            return *this;
        }

        vorbis_channel() noexcept:
            _channels(0),
            _sampleRate(0),
            _byteRate(0),
            _size(0),
            _dataStart(0),
            _time(0),
            _format(static_cast<format>(0)),
            _handle(nullptr){}

        vorbis_channel(const std::string& path) noexcept;

        vorbis_channel(const void * data, std::size_t size) noexcept;

        virtual ~vorbis_channel() noexcept;                

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
