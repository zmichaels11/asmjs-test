#include "audio/vorbis_file_channel.hpp"

#include <cstdio>

#include <string>

#define STB_VORBIS_NO_PUSHDATA_API
#define STB_VORBIS_NO_INTEGER_CONVERSION

#include "stb_vorbis.h"

namespace audio {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    vorbis_file_channel::vorbis_file_channel(const std::string& path) noexcept {
        int err;
        auto handle = stb_vorbis_open_filename(path.c_str(), &err, nullptr);

        if (err) {
            _onError("Unable to open vorbis file: " + path);
        }

        _handle = handle;

        auto info = stb_vorbis_get_info(handle);

        _channels = info.channels;
        _sampleRate = info.sample_rate;      
        _format = (_channels == 1)
            ? format::MONO_FLOAT32
            : format::STEREO_FLOAT32;

        _time = stb_vorbis_stream_length_in_seconds(handle);
        _byteRate = _sampleRate * _channels * 4;
    }

    vorbis_file_channel::~vorbis_file_channel() noexcept {
        if (_handle) {
            stb_vorbis_close(reinterpret_cast <stb_vorbis *> (_handle));
        }
    }

    void vorbis_file_channel::seekStart() noexcept {
        stb_vorbis_seek_start(reinterpret_cast <stb_vorbis *> (_handle));
    }

    void vorbis_file_channel::seek(unsigned int sample) noexcept {
        stb_vorbis_seek(reinterpret_cast <stb_vorbis *> (_handle), sample);
    }

    float vorbis_file_channel::getLength() const noexcept {
        return _time;
    }

    int vorbis_file_channel::getChannels() const noexcept {
        return _channels;
    }

    int vorbis_file_channel::getSampleRate() const noexcept {
        return _sampleRate;
    }

    int vorbis_file_channel::getByteRate() const noexcept {
        return _byteRate;
    }

    int vorbis_file_channel::getBitsPerSample() const noexcept {
        return 32;
    }

    format vorbis_file_channel::getFormat() const noexcept {
        return _format;
    }

    bool vorbis_file_channel::read(void * dst, std::size_t& n) noexcept{
        auto floats = reinterpret_cast<float *> (dst);
        auto numFloats = n / sizeof(float);
        auto handle = reinterpret_cast<stb_vorbis *> (_handle);
        auto read = stb_vorbis_get_samples_float_interleaved(handle, _channels, floats, numFloats);

        n = read * sizeof(float);

        return (read != 0);
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::printf("[AL] Vorbis error: %s\n", msg.c_str());
            __builtin_trap();
        }
    }
}