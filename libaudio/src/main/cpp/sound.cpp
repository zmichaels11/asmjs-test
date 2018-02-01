#include "audio/sound.hpp"

#include <cstddef>

#include <iostream>
#include <memory>
#include <string>

#include "audio/vorbis_file_channel.hpp"
#include "audio/wave_file_channel.hpp"
#include "util.hpp"

namespace audio {
    namespace {
        void _onError(const std::string& msg) noexcept;
    }

    sound::sound(const std::string& path, float loopOffset) noexcept {
        if (util::hasSuffix(path, "ogg")) {
            _channel = std::make_unique<vorbis_file_channel> (path);
        } else if (util::hasSuffix(path, "wav")) {
            _channel = std::make_unique<wave_file_channel> (path);
        } else {
            _onError("Unsupported file type: " + path);
        }

        _state = sound_state::STOPPED;
        _loopSample = (unsigned int) (loopOffset * _channel->getSampleRate());
        _bufferSize = util::alignUp(_channel->getByteRate() / 20, 4096);
        _format = _channel->getFormat();
        _sampleRate = _channel->getSampleRate();
    }

    void sound::onFrame() noexcept {
        if (_state == sound_state::STOPPED) {
            return;
        }

        auto readySize = _source.getBuffersProcessed();

        if (readySize == 0) {
            // no buffers are processed
            return;
        }
        
        auto ready = std::make_unique<buffer[]> (readySize);

        _source.unqueueBuffers(ready.get(), readySize);

        auto eof = false;

        for (auto pBuffer = ready.get(); pBuffer != (ready.get() + readySize); pBuffer++) {
            auto transfer = std::make_unique<char[]> (_bufferSize);

            if (_state == sound_state::PLAYING) {
                auto size = _bufferSize;
                auto eof = !_channel->read(transfer.get(), size);
                
                pBuffer->setData(_format, transfer.get(), size, _sampleRate);
                _source.queueBuffers(pBuffer);

                if (eof) {
                    _state = sound_state::STOPPED;
                    break;
                }
            } else {
                auto pTfr = transfer.get();
                auto end = transfer.get() + _bufferSize;

                while (pTfr != end) {
                    auto remaining = static_cast<std::size_t> (end - pTfr);

                    if (!_channel->read(pTfr, remaining)) {
                        _channel->seek(_loopSample);
                        pTfr += remaining;
                    } else {
                        break;
                    }
                }

                pBuffer->setData(_format, transfer.get(), _bufferSize, _sampleRate);
                _source.queueBuffers(pBuffer);
            }
        }
    }

    void sound::setPosition(float x, float y, float z) noexcept {
        _source.setPosition(x, y, z);
    }

    void sound::setVelocity(float x, float y, float z) noexcept {
        _source.setVelocity(x, y, z);
    }

    void sound::setDirection(float x, float y, float z) noexcept {
        _source.setDirection(x, y, z);
    }

    void sound::setGain(float value) noexcept {
        _source.setGain(value);
    }

    void sound::setPitch(float value) noexcept {
        _source.setPitch(value);
    }

    sound_state sound::getState() const noexcept {
        return _state;
    }

    void sound::play() noexcept {
        int i = 0;

        for (; i < 3; i++) {
            auto transfer = std::make_unique<char[]> (_bufferSize);
            auto size = _bufferSize;
            auto eof = !_channel->read(transfer.get(), size);

            _buffers[i].setData(_format, transfer.get(), size, _sampleRate);            

            if (eof) {
                break;
            }
        }

        _source.queueBuffers(_buffers, i);
        _state = sound_state::PLAYING;
        _source.play();
    }

    void sound::loop() noexcept {
        int i;

        for (; i < 3; i++) {
            auto transfer = std::make_unique<char[]> (_bufferSize);
            auto pTfr = transfer.get();
            auto end = transfer.get() + _bufferSize;
            
            while (pTfr != end) {
                auto remaining = static_cast<std::size_t> (end - pTfr);

                if (!_channel->read(pTfr, remaining)) {
                    _channel->seek(_loopSample);
                    pTfr += remaining;
                } else {
                    break;
                }
            }

            _buffers[i].setData(_format, transfer.get(), _bufferSize, _sampleRate);            
        }

        _source.queueBuffers(_buffers, i);
        _state = sound_state::LOOPING;
        _source.play();
    }

    void sound::stop() noexcept {
        _state = sound_state::STOPPED;
        _source.setGain(0.0F);
    }

    namespace {
        void _onError(const std::string& msg) noexcept {
            std::cerr << "[AL] sound error: " << msg << std::endl;
            __builtin_trap();
        }
    }
}