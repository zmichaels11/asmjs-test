#include "audio/wave_file_channel.hpp"

#include <cstdio>
#include <iostream>
#include <memory>

namespace audio {
    static short _parseShort(char * c) {
        auto ps = reinterpret_cast<short *> (c);

        return *ps;
    }

    static int _parseInt(char * c) {
        auto pi = reinterpret_cast<int *> (c);

        return *pi;
    }

    static int _parseIntBE(char * c) {
        char tmp[4];

        tmp[0] = c[3];
        tmp[1] = c[2];
        tmp[2] = c[1];
        tmp[3] = c[0];

        auto pi = reinterpret_cast<int *> (tmp);

        return *pi;
    }

    wave_file_channel::wave_file_channel(const std::string& path) {
        _pFile = std::fopen(path.c_str(), "r");

        char riffHeader[12];
        char * p;

        if (std::fread(riffHeader, 1, 12, _pFile) != 12) {
            printf("Stream terminated early!\n");
            __builtin_trap();
        }

        p = riffHeader;

        constexpr int RIFF = 0x52494646;
        int riff = _parseIntBE(p);

        if (riff != RIFF) {
            printf("Container is not RIFF! Expected: 0x%08x but got 0x%08x\n", RIFF, riff);
            __builtin_trap();
        }

        constexpr int WAVE = 0x57415645;
        int wave = _parseIntBE(p + 8);

        if (wave != WAVE) {
            printf("Audio format is not WAVE! Expected: 0x%08x but got 0x%08x\n", WAVE, wave);
            __builtin_trap();
        }
        
        while (!parseSubchunk()) {
            //process
        }

        _dataStart = std::ftell(_pFile);
    }

    void wave_file_channel::close() {
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
        auto buffer = std::make_unique<char>(chunkSize);
        auto data = buffer.get();

        std::fread(data, 1, chunkSize, _pFile);

        auto audioFormat = _parseShort(data);

        _channels = _parseShort(data + 2);
        _sampleRate = _parseInt(data + 4);
        _byteRate = _parseInt(data + 8);
        //blockAlign = _parseShort(data + 8);
        _bitsPerSample = _parseShort(data + 14);

        constexpr int _PCM = 0x0001;

        if (audioFormat != _PCM) {
            short extSize = _parseShort(data + 16);

            if (extSize == 22) {
                //validBitsPerSample = _parseShort(data + 18);
                //channelMask = _parseInt(data + 20);
                audioFormat = _parseShort(data + 24);
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
                                printf("Unsupported audio format!\n");
                                __builtin_trap();
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
                                printf("Unsupported audio format!\n");
                                __builtin_trap();                            
                        }
                        break;
                    default:
                        printf("Unsupported audio format!\n");
                        __builtin_trap();                        
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
                                printf("Unsupported audio format!\n");
                                __builtin_trap();
                        }
                        break;
                    case 2:
                        switch (_bitsPerSample) {
                            case 32:
                                _format = format::STEREO_FLOAT32;
                                break;
                            default:
                                printf("Unsupported audio format!\n");
                                __builtin_trap();
                        }
                        break;
                    default:
                        printf("Unsupported audio format!\n");
                        __builtin_trap();                        
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
                        printf("Unsupported audio format!\n");
                        __builtin_trap();
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
                        printf("Unsupported audio format!\n");
                        __builtin_trap();
                }
                break;
            default:
                printf("Unsupported audio format!\n");
                __builtin_trap();
        }
    }

    void wave_file_channel::skipSubchunk(unsigned int chunkSize) {
        std::fseek(_pFile, chunkSize, SEEK_CUR);
    }

    bool wave_file_channel::parseSubchunk() {
        char header[8];

        if (std::fread(header, 1, 8, _pFile) != 8) {
            printf("Stream terminated early!\n");
            __builtin_trap();
        }

        auto chunkId = _parseIntBE(header);
        auto chunkSize = _parseInt(header + 4);

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