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

    std::size_t source::getProcessedBuffers() const noexcept {
        ALint nBuffers = 0;

        alGetSourcei(_handle, AL_BUFFERS_PROCESSED, &nBuffers);

        return static_cast<std::size_t> (nBuffers);
    }

    void source::unqueueBuffers(buffer * pBuffers, std::size_t nBuffers) const noexcept {
        auto tmp = std::make_unique<ALuint[]> (nBuffers);

        alSourceUnqueueBuffers(_handle, nBuffers, tmp.get());

        for (decltype(nBuffers) i = 0; i < nBuffers; i++) {            
            pBuffers[i] = buffer(tmp[i]);            
        }
    }

    void source::queueBuffers(const buffer * pBuffers, std::size_t nBuffers) const noexcept {
        auto tmp = std::make_unique<ALuint[]> (nBuffers);

        for (decltype(nBuffers) i = 0; i < nBuffers; i++) {
            tmp[i] = pBuffers[i];
        }

        alSourceQueueBuffers(_handle, static_cast<ALsizei> (nBuffers), tmp.get());
    }
    
    void source::play() const noexcept {
        alSourcePlay(_handle);
    }
}
