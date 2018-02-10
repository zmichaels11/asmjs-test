#include "pch.h"
#include "audio/buffered_channel.hpp"

#include <cstring>

namespace audio {
    buffered_channel::buffered_channel(sound_channel&& base) noexcept {
        _format = base.getFormat();
        _sampleRate = base.getSampleRate();
        _channels = base.getChannels();
        _bitsPerSample = base.getBitsPerSample();
        _byteRate = base.getByteRate();
        _length = base.getLength();
        _currentBuffer = 0;
        _currentOffset = 0;        
        _bufferSize = util::alignUp(base.getByteRate() / 20, 4096);

        bool eof = false;

        while (!eof) {
            auto buffer = std::vector<char>();

            buffer.reserve(_bufferSize);

            std::size_t size = _bufferSize;

            eof = base.read(buffer.data(), size);

            buffer.resize(size);
            _buffers.push_back(buffer);
        }
    }

    buffered_channel::buffered_channel(const buffered_channel& other) noexcept {
        
    }

    int buffered_channel::getChannels() const noexcept {
        return _channels;
    }

    int buffered_channel::getSampleRate() const noexcept {
        return _sampleRate;
    }

    int buffered_channel::getBitsPerSample() const noexcept {
        return _bitsPerSample;
    }

    int buffered_channel::getByteRate() const noexcept {
        return _byteRate;
    }

    format buffered_channel::getFormat() const noexcept {
        return _format;
    }

    float buffered_channel::getLength() const noexcept {
        return _length;
    }

    void buffered_channel::seekStart() noexcept {
        _currentBuffer = 0;
        _currentOffset = 0;
    }

    void buffered_channel::seek(unsigned int sample) noexcept {
        auto byteOffset = _byteRate * sample;

        _currentBuffer = 0;

        while (byteOffset > _bufferSize) {
            byteOffset -= _bufferSize;
            _currentBuffer++;
        }

        _currentOffset = byteOffset;
    }

    bool buffered_channel::read(void * dst, std::size_t& n) noexcept {
        //TODO: implement
        return false;
    }
}