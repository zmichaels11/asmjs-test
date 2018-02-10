#include "pch.h"
#include "audio/source.hpp"

#include "audio/buffer.hpp"

namespace audio {
    source::source() noexcept {
        alGenSources(1, &_handle);
    }

    source::~source() noexcept {
        alDeleteSources(1, &_handle);        
    }

    void source::setGain(float value) const noexcept {
        alSourcef(_handle, AL_GAIN, value);
    }

    void source::setPitch(float value) const noexcept {
        alSourcef(_handle, AL_PITCH, value);
    }

    void source::setPosition(float x, float y, float z) const noexcept {
        alSource3f(_handle, AL_POSITION, x, y, z);
    }

    void source::setVelocity(float x, float y, float z) const noexcept {
        alSource3f(_handle, AL_VELOCITY, x, y, z);
    }

    void source::setDirection(float x, float y, float z) const noexcept {
        alSource3f(_handle, AL_DIRECTION, x, y, z);
    }

    std::size_t source::gc() const noexcept {
        ALint nBuffers = 0;

        alGetSourcei(_handle, AL_BUFFERS_PROCESSED, &nBuffers);

        switch (nBuffers) {
            case 0:
                break;
            case 1: {
                ALuint buffer = 0;

                alSourceUnqueueBuffers(_handle, 1, &buffer);
                alDeleteBuffers(1, &buffer);
            } break;
            default: {
                auto pBuffers = std::make_unique<ALuint[]> (nBuffers);

                alSourceUnqueueBuffers(_handle, nBuffers, pBuffers.get());
                alDeleteBuffers(nBuffers, pBuffers.get());
            } break;
        }

        return static_cast<std::size_t> (nBuffers);
    }
    
    void source::queueBuffer(audio::buffer&& buffer) const noexcept {        
        alSourceQueueBuffers(_handle, 1, &buffer._handle);
        // steal the handle
        buffer._handle = 0;
    }    

    void source::play() const noexcept {
        alSourcePlay(_handle);
    }
}
