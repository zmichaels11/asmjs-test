#include "audio/wave_file_channel.hpp"

#include <cstdio>
#include <cstdint>

namespace audio {
    static void _onError(const std::string& msg) {        
        #ifdef __EMSCRIPTEN__
            printf("ERR: %s\n", msg.c_str());
            __builtin_trap();
        #else
            throw msg;
        #endif
    }

    wave_file_channel::wave_file_channel(const std::string& path) {
        if ((_pFile = std::fopen(path.c_str(), "rb")) == nullptr) {
            _onError("Could not open file: " + path);
        }

        constexpr std::int32_t RIFF = 0x46464952;
        std::int32_t riff = 0;

        if (std::fread(&riff, 4, 1, _pFile) != 1) {
            _onError("Stream terminated before reading container type!");
        }

        if (riff != RIFF) {            
            _onError("Container is not RIFF!");
        }

        if (std::fseek(_pFile, 4, SEEK_CUR) != 0) {
            _onError("Error skipping 4 bytes!");
        }

        constexpr std::int32_t WAVE = 0x45564157;
        std::int32_t wave = 0;

        if (std::fread(&wave, 4, 1, _pFile) != 1) {
            _onError("Stream terminated before reading audio format type!");
        }

        if (wave != WAVE) {
            _onError("Audio format is not WAVE!");
        }
        
        while (!parseSubchunk()) {
            //process
        }

        _dataStart = std::ftell(_pFile);
    }

    wave_file_channel::~wave_file_channel() {
        if (_pFile) {
            std::fclose(_pFile);
            _pFile = nullptr;
        }
    }

    void wave_file_channel::seekStart() {

    }

    void wave_file_channel::seek(unsigned int sample) {

    }

    float wave_file_channel::getLength() const {
        return 0.0F;
    }

    int wave_file_channel::getSampleRate() const {
        return _sampleRate;
    }

    int wave_file_channel::getChannels() const {
        return _channels;
    }

    int wave_file_channel::getBitsPerSample() const {
        return _bitsPerSample;
    }

    int wave_file_channel::getByteRate() const {
        return _byteRate;
    }

    format wave_file_channel::getFormat() const {
        return static_cast<format>(0);
    }

    int wave_file_channel::read(char * dst, unsigned int n) {
        return 0;
    }

    bool wave_file_channel::isOpen() const {
        return _pFile != nullptr;
    }

    void wave_file_channel::parseFormatSubchunk(unsigned int chunkSize) {        
        const auto endOfChunk = std::ftell(_pFile) + chunkSize;
        std::int16_t audioFormat = 0;        

        if (std::fread(&audioFormat, 2, 1, _pFile) != 1) {
            _onError("Stream terminated before reading audio format in format subchunk!");
        }

        if (std::fread(&_channels, 2, 1, _pFile) != 1) {
            _onError("Stream terminated before reading channels in format subchunk!");
        }
        
        if (std::fread(&_sampleRate, 4, 1, _pFile) != 1) {
            _onError("Stream terminated before reading sample rate in format subchunk!");
        }

        if (std::fread(&_byteRate, 4, 1, _pFile) != 1) {
            _onError("Stream terminated before reading byteRate in format subchunk!");
        }

        // skip blockAlign
        if (std::fseek(_pFile, 2, SEEK_CUR) != 0) {
            _onError("Error skipping 2 bytes!");
        }

        if (std::fread(&_bitsPerSample, 2, 1, _pFile) != 1) {
            _onError("Stream terminated before reading bitsPerSample in format subchunk!");
        }

        constexpr std::int16_t _PCM = 0x0001;

        if (audioFormat != _PCM) {
            std::int16_t extSize = 0;

            if (std::fread(&extSize, 2, 1, _pFile) != 1) {
                _onError("Stream terminated before reading extSize in format subchunk!");
            }

            if (extSize == 22) {
                if (std::fseek(_pFile, 6, SEEK_CUR) != 0) {
                    _onError("Error skipping 2 bytes!");
                }
                
                if (!std::fread(&audioFormat, 2, 1, _pFile) != 1) {
                    _onError("Stream terminated before reading audioFormat it format subchunk!");
                }
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

        std::fseek(_pFile, endOfChunk, SEEK_SET);
    }

    void wave_file_channel::skipSubchunk(unsigned int chunkSize) {
        std::fseek(_pFile, chunkSize, SEEK_CUR);
    }

    bool wave_file_channel::parseSubchunk() {        
        std::int32_t chunkId = 0;

        if (std::fread(&chunkId, 4, 1, _pFile) != 1) {
            _onError("Stream terminated early!");
        }
        
        std::int32_t chunkSize = 0;

        if (std::fread(&chunkSize, 4, 1, _pFile) != 1) {
            _onError("Stream terminated early!");
        }

        switch (chunkId) {
            case 0x20746d66:
                parseFormatSubchunk(chunkSize);
                return false;
            case 0x61746164:
                _size = chunkSize;
                return true;
            default:
                skipSubchunk(chunkSize);
                return false;
        }
    }
}