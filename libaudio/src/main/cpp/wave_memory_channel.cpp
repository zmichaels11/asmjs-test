#include "pch.h"
#include "audio/wave_memory_channel.hpp"

#include <cstring>

#include <sstream>

namespace audio {
    namespace {
        void  _onError(const std::string& msg) noexcept;
    }

    wave_memory_channel::wave_memory_channel(const char * data, std::size_t len) noexcept {
        constexpr std::int32_t RIFF = 0x46464952;
        constexpr std::int32_t WAVE = 0x45564157;

        std::int32_t riff = 0;
        std::int32_t wave = 0;

        _data = data;
        _offset = 0;
        _len = len;

        std::memcpy(&riff, _data, 4); _offset += 4;
        _offset += 4;
        std::memcpy(&wave, _data + _offset, 4); _offset += 4;

        if (riff != RIFF) {
            std::stringstream err;

            err << "Container is not RIFF! Expected: "
                << RIFF 
                << " but got: "
                << riff;

            _onError(err.str());
        }

        if (wave != WAVE) {
            _onError("Audio format is not WAVE!");
        }

        while (!parseSubchunk()) {
            // process
        }

        _dataStart = _offset;
    }

    void wave_memory_channel::seekStart() noexcept {
        _offset = _dataStart;
    }

    float wave_memory_channel::getLength() const noexcept {
        return static_cast<float> (_size) / static_cast<float> (_byteRate);
    }

    int wave_memory_channel::getSampleRate() const noexcept {
        return _sampleRate;
    }

    int wave_memory_channel::getChannels() const noexcept {
        return _channels;
    }

    int wave_memory_channel::getBitsPerSample() const noexcept {
        return _bitsPerSample;
    }

    int wave_memory_channel::getByteRate() const noexcept {
        return _byteRate;
    }

    format wave_memory_channel::getFormat() const noexcept {
        return _format;
    }

    bool wave_memory_channel::read(void * dst, std::size_t& n) noexcept {
        auto remaining = _len - _offset;

        if (n > remaining) {
            n = static_cast<std::size_t> (remaining);
            std::memcpy(dst, _data + _offset, n);
            _offset = _len;
            return false;
        } else if (n == remaining) {
            std::memcpy(dst, _data + _offset, n);
            _offset = _len;
            return false;
        } else {
            std::memcpy(dst, _data + _offset, n);
            _offset += n;
            return true;
        }
    }

    void wave_memory_channel::parseFormatSubchunk(unsigned int chunkSize) noexcept {
        std::int16_t audioFormat = 0;

        std::memcpy(&audioFormat, _data + _offset, 2);                      _offset += 2;
        std::memcpy(&_channels, _data + _offset, sizeof(_channels));        _offset += sizeof(_channels);
        std::memcpy(&_sampleRate, _data + _offset, sizeof(_sampleRate));    _offset += sizeof(_sampleRate);
        std::memcpy(&_byteRate, _data + _offset, sizeof(_byteRate));        _offset += sizeof(_byteRate);

        _offset += 2;

        std::memcpy(&_bitsPerSample, _data + _offset, sizeof(_byteRate));   _offset += sizeof(_byteRate);

        constexpr std::int16_t PCM = 0x0001;

        if (audioFormat != PCM) {
            std::int16_t extSize = 0;

            std::memcpy(&extSize, _data + _offset, 2); _offset += 2;

            if (extSize == 22) {
                _offset += 6;
                std::memcpy(&audioFormat, _data + _offset, 2); _offset += 2;
            }
        }

        switch (audioFormat) {
            case 0x0001:
                switch (_channels) {
                    case 1:
                        switch (_bitsPerSample) {
                            case 8:
                                _format = format::MONO8;
                                break;
                            case 16:
                                _format = format::MONO16;
                                break;
                            default:
                                _onError("Unsupported audio format!");
                        } break;
                    case 2:
                        switch (_bitsPerSample) {
                            case 8:
                                _format = format::STEREO8;
                                break;
                            case 16:
                                _format = format::STEREO16;
                                break;
                            default:
                                _onError("Unsupported audio format!");                                
                        } break;
                    default:
                        _onError("Unsupported audio format!");
                } break;
            case 0x0003:
                switch (_channels) {
                    case 1:
                        switch (_bitsPerSample) {
                            case 32:
                                _format = format::MONO_FLOAT32;
                                break;
                            default:
                                _onError("Unsupported audio format!");
                        } break;
                    case 2:
                        switch (_bitsPerSample) {
                            case 32:
                                _format = format::STEREO_FLOAT32;
                                break;
                            default:
                                _onError("Unsupported audio format!");
                        } break;
                    default:
                        _onError("Unsupported audio format!");
                } break;
            case 0x0006:
                switch (_channels) {
                    case 1:
                        _format = format::MONO_ALAW;
                        break;
                    case 2:
                        _format = format::STEREO_ALAW;
                        break;
                    default:
                        _onError("Unsupported audio format!");                    
                } break;
            case 0x0007:
                switch (_channels) {
                    case 1:
                        _format = format::MONO_ULAW;
                        break;
                    case 2:
                        _format = format::STEREO_ULAW;
                        break;
                    default:
                        _onError("Unsupported audio format!");
                } break;
            default:
                _onError("Unsupported audio format!");
        }
    }

    void wave_memory_channel::seek(unsigned int sample) noexcept {
        auto seekPos = _channels * _bitsPerSample / 8 * sample;

        _offset = _dataStart + seekPos;
    }

    void wave_memory_channel::skipSubchunk(unsigned int chunkSize) noexcept {
        _offset += chunkSize;
    }

    bool wave_memory_channel::parseSubchunk() noexcept {
        std::int32_t chunkId;
        std::int32_t chunkSize;

        std::memcpy(&chunkId, _data + _offset, 4);      _offset += 4;
        std::memcpy(&chunkSize, _data + _offset, 4);    _offset += 4;

        chunkId = __builtin_bswap32(chunkId);

        switch (chunkId) {
            case 0x666d7420:
                parseFormatSubchunk(chunkSize);
                return false;
            case 0x64617461:
                _size = chunkSize;
                return true;
            default:
                skipSubchunk(chunkSize);
                return false;
        }
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[AL] wave_memory_channel error: " << msg << std::endl;
            __builtin_trap();
        }
    }
}