#include "audio/vorbis_file_channel.hpp"
#include <cstdint>

#include <iostream>
#include <sstream>
#include <string>

#include "audio/stb_vorbis.hpp"

namespace audio {
    static void _onError(const std::string& msg) {
        std::cerr << "Err: " << msg << std::endl;
        __builtin_trap();
    }

    vorbis_file_channel::vorbis_file_channel(const std::string& path) {
        int err = 0;

        _handle = stb_vorbis_open_filename(path.c_str(), &err, nullptr);

        auto info = stb_vorbis_get_info(_handle);

        _channels = info.channels;
        _sampleRate = info.sample_rate;      
        _format = (_channels == 1)
            ? format::MONO_FLOAT32
            : format::STEREO_FLOAT32;

        _time = stb_vorbis_stream_length_in_seconds(_handle);
        _byteRate = _sampleRate * _channels * 4;
        std::cout << "Opened vorbis handle: " << _handle << std::endl;
    }

    vorbis_file_channel::~vorbis_file_channel() {
        if (_handle) {
            std::cout << "Closing vorbis handle: " << _handle << std::endl;
            stb_vorbis_close(_handle);
        }
    }

    void vorbis_file_channel::seekStart() {
        stb_vorbis_seek_start(_handle);
    }

    void vorbis_file_channel::seek(unsigned int sample) {
        stb_vorbis_seek(_handle, sample);
    }

    float vorbis_file_channel::getLength() const {
        return _time;
    }

    int vorbis_file_channel::getChannels() const {
        return _channels;
    }

    int vorbis_file_channel::getSampleRate() const {
        return _sampleRate;
    }

    int vorbis_file_channel::getByteRate() const {
        return _byteRate;
    }

    int vorbis_file_channel::getBitsPerSample() const {
        return 32;
    }

    format vorbis_file_channel::getFormat() const {
        return _format;
    }

    bool vorbis_file_channel::read(float * dst, std::size_t& n) {
        auto read = stb_vorbis_get_samples_float_interleaved(_handle, _channels, dst, int(n));

        n = std::size_t(read);

        return (read != 0);
    }

    bool vorbis_file_channel::read(char * dst, std::size_t& n) {
        auto floats = reinterpret_cast<float *> (dst);
        int numFloats = n / sizeof(float);
        int read = stb_vorbis_get_samples_float_interleaved(_handle, _channels, floats, numFloats);

        n = read * sizeof(float);

        return (read != 0);
    }
}