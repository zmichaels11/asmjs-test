#include "audio/sound.hpp"

#include <cstddef>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "audio/vorbis_file_channel.hpp"
#include "audio/wave_file_channel.hpp"
#include "util.hpp"

namespace audio {
    namespace {
        static bool has_suffix(const std::string &str, const std::string &suffix) {
            return str.size() >= suffix.size() &&
                str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
        }
    }

    sound::sound(const std::string& path, float loopOffset) {
        if (has_suffix(path, "ogg")) {
            _channel = std::make_unique<vorbis_file_channel> (path);
        } else if (has_suffix(path, "wav")) {
            _channel = std::make_unique<wave_file_channel> (path);
        } else {
            std::cerr << "Unsupported file type: " << path << std::endl;
            __builtin_trap();
        }

        _state = sound_state::STOPPED;
        _loopSample = (unsigned int) (loopOffset * _channel->getSampleRate());
        _bufferSize = util::alignUp(_channel->getByteRate() / 20, 4096);
        _format = _channel->getFormat();
        _sampleRate = _channel->getSampleRate();
    }

    void sound::onFrame() {
        if (_state == sound_state::STOPPED) {
            return;
        }

        std::vector<buffer> ready;
        bool eof;

        if (!_source.unqueueBuffers(ready)) {
            return;
        }

        for (auto&& buffer: ready) {
            char transfer[_bufferSize];                        

            if (_state == sound_state::PLAYING) {
                std::size_t size = _bufferSize;
                bool eof = !_channel->read(transfer, size);
                
                buffer.setData(_format, transfer, size, _sampleRate);
                _source.queueBuffer(buffer);

                if (eof) {
                    _state = sound_state::STOPPED;
                    break;
                }
            } else {
                char * pTfr = transfer;
                char * end = transfer + _bufferSize;

                while (pTfr != end) {
                    std::size_t remaining = end - pTfr;

                    if (!_channel->read(pTfr, remaining)) {
                        _channel->seek(_loopSample);
                        pTfr += remaining;
                    } else {
                        break;
                    }
                }

                buffer.setData(_format, transfer, _bufferSize, _sampleRate);
                _source.queueBuffer(buffer);
            }
        }
    }

    void sound::setPosition(float x, float y, float z) {
        _source.setPosition(x, y, z);
    }

    void sound::setVelocity(float x, float y, float z) {
        _source.setVelocity(x, y, z);
    }

    void sound::setDirection(float x, float y, float z) {
        _source.setDirection(x, y, z);
    }

    void sound::setGain(float value) {
        _source.setGain(value);
    }

    void sound::setPitch(float value) {
        _source.setPitch(value);
    }

    sound_state sound::getState() const {
        return _state;
    }

    void sound::play() {                
        for (int i = 0; i < 3; i++) {
            char transfer[_bufferSize];
            std::size_t size = _bufferSize;
            bool eof = !_channel->read(transfer, size);

            _buffers[i].setData(_format, transfer, size, _sampleRate);
            _source.queueBuffer(_buffers[i]);

            if (eof) {
                break;
            }
        }

        _state = sound_state::PLAYING;
        _source.play();
    }

    void sound::loop() {
        for (int i = 0; i < 3; i++) {
            char transfer[_bufferSize];
            char * pTfr = transfer;
            char * end = transfer + _bufferSize;
            
            while (pTfr != end) {
                std::size_t remaining = end - pTfr;

                if (!_channel->read(pTfr, remaining)) {
                    _channel->seek(_loopSample);
                    pTfr += remaining;
                } else {
                    break;
                }
            }

            _buffers[i].setData(_format, transfer, _bufferSize, _sampleRate);
            _source.queueBuffer(_buffers[i]);
        }

        _state = sound_state::LOOPING;
        _source.play();
    }

    void sound::stop() {
        _state = sound_state::STOPPED;
        _source.setGain(0.0F);
    }
}