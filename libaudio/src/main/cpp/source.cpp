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

    std::size_t source::getBuffersQueued() const noexcept {
        ALint out = 0;

        alGetSourcei(_handle, AL_BUFFERS_QUEUED, &out);

        return static_cast<std::size_t> (out);
    }

    std::size_t source::getBuffersProcessed() const noexcept {
        ALint out = 0;

        alGetSourcei(_handle, AL_BUFFERS_PROCESSED, &out);

        return static_cast<std::size_t> (out);
    }

    void source::unqueueBuffers(buffer * pBuffers, std::size_t nbuffers) const noexcept {
        if (pBuffers != nullptr && nbuffers > 0) {            
            auto buffers = std::make_unique<ALuint[]> (nbuffers);
        
            alSourceUnqueueBuffers(_handle, static_cast<ALsizei> (nbuffers), buffers.get());        

            for (int i = 0; i < nbuffers; i++) {
                pBuffers[i] = buffer(buffers[i]);
            }
        }
    }

    void source::queueBuffers(const buffer * pBuffers, std::size_t nBuffers) const noexcept {
        if (nBuffers) {
            auto buffers = std::make_unique<ALuint[]> (nBuffers);

            for (int i = 0; i < nBuffers; i++) {
                buffers[i] = pBuffers[i]._handle;
            }

            alSourceQueueBuffers(_handle, static_cast<ALsizei> (nBuffers), buffers.get());
        }
    }

    void source::play() const noexcept {
        alSourcePlay(_handle);
    }
}
