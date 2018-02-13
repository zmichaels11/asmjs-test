#include "pch.h"
#include "audio/wave_file_channel.hpp"

#include <fstream>
#include <sstream>

namespace audio {    
    namespace {
        void _onError(const std::string& msg) {
            std::cerr << "[AL] wave_file_channel error: " << msg << std::endl;
            __builtin_trap();        
        }    
    }    

    wave_file_channel::wave_file_channel(const std::string& path) noexcept {
        _file.open(path, std::ifstream::binary);

        if (!_file) {
            _onError("Failed to open file: " + path);
        }

        constexpr std::int32_t RIFF = 0x46464952;
        constexpr std::int32_t WAVE = 0x45564157;

        std::int32_t riff = 0;
        std::int32_t wave = 0;

        _file.read(reinterpret_cast<char *> (&riff), sizeof(riff));        
        _file.seekg(4, std::ifstream::cur);
        _file.read(reinterpret_cast<char *> (&wave), sizeof(wave));

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
            //process
        }

        _dataStart = _file.tellg();
    }

    void wave_file_channel::seekStart() noexcept {
        _file.seekg(_dataStart);
    }

    void wave_file_channel::seek(unsigned int sample) noexcept {
        auto seekPos = _channels * _bitsPerSample / 8 * sample;

        _file.seekg(seekPos, std::ifstream::cur);            
    }

    float wave_file_channel::getLength() const noexcept {
        return static_cast<float> (_size) / static_cast<float> (_byteRate);
    }

    int wave_file_channel::getSampleRate() const noexcept {
        return _sampleRate;
    }

    int wave_file_channel::getChannels() const noexcept {
        return _channels;
    }

    int wave_file_channel::getBitsPerSample() const noexcept {
        return _bitsPerSample;
    }

    int wave_file_channel::getByteRate() const noexcept {
        return _byteRate;
    }

    format wave_file_channel::getFormat() const noexcept {
        return _format;
    }

    bool wave_file_channel::read(void * dst, std::size_t& n) noexcept {
        auto chars = reinterpret_cast<char *> (dst);

        if (_file.read(chars, n)) {
            return true;
        } else {
            n = _file.gcount();
            return false;
        }        
    }

    void wave_file_channel::parseFormatSubchunk(unsigned int chunkSize) noexcept {                
        std::int16_t audioFormat;

        _file.read(reinterpret_cast<char *> (&audioFormat), sizeof(audioFormat));
        _file.read(reinterpret_cast<char *> (&_channels), sizeof(_channels));
        _file.read(reinterpret_cast<char *> (&_sampleRate), sizeof(_sampleRate));
        _file.read(reinterpret_cast<char *> (&_byteRate), sizeof(_byteRate));        
        // skip blockAlign
        _file.seekg(2, std::ifstream::cur);
        _file.read(reinterpret_cast<char *> (&_bitsPerSample), sizeof(_bitsPerSample));

        constexpr std::int16_t _PCM = 0x0001;

        if (audioFormat != _PCM) {
            std::int16_t extSize;

            _file.read(reinterpret_cast<char *> (&extSize), sizeof(extSize));

            if (extSize == 22) {                
                _file.seekg(6, std::ifstream::cur);
                _file.read(reinterpret_cast <char*> (audioFormat), sizeof(audioFormat));
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
                        }
                        break;
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
                        }
                        break;
                    default:
                        _onError("Unsupported audio format!");
                }
                break;
            case 0x0003:
                switch (_channels) {
                    case 1:
                        switch (_bitsPerSample) {
                            case 32:
                                _format = format::MONO_FLOAT32;
                                break;
                            default:
                                _onError("Unsupported audio format!");
                        }
                        break;
                    case 2:
                        switch (_bitsPerSample) {
                            case 32:
                                _format = format::STEREO_FLOAT32;
                                break;
                            default:
                                _onError("Unsupported audio format!");
                        }
                        break;
                    default:
                        _onError("Unsupported audio format!");
                }
                break;
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
                }
                break;
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
                }
                break;
            default:
                _onError("Unsupported audio format!");
        }
    }

    void wave_file_channel::skipSubchunk(unsigned int chunkSize) noexcept {        
        _file.seekg(chunkSize, std::ifstream::cur);
    }

    bool wave_file_channel::parseSubchunk() noexcept {
        std::int32_t chunkId;        
        std::int32_t chunkSize;

        _file.read(reinterpret_cast<char *> (&chunkId), sizeof(chunkId));
        _file.read(reinterpret_cast<char *> (&chunkSize), sizeof(chunkSize));

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
}
