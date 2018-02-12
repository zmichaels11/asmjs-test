#include "pch.h"
#include "audio/source.hpp"

#include "audio.hpp"
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
        box(pBuffers, nBuffers, [ this ](auto pBuffers, auto nBuffers) {
            alSourceUnqueueBuffers(*this, nBuffers, pBuffers);
        });
    }

    source_state source::getState() const noexcept {
        ALint state = 0;

        alGetSourcei(_handle, AL_SOURCE_STATE, &state);

        return static_cast<source_state> (state);
    }

    void source::queueBuffers(const buffer * pBuffers, std::size_t nBuffers) const noexcept {
        unbox(pBuffers, nBuffers, [ this ](auto pBuffers, auto nBuffers) {
            alSourceQueueBuffers(*this, nBuffers, pBuffers);
        });   
    }
    
    void source::play() const noexcept {
        alSourcePlay(_handle);
    }

    void source::pause() const noexcept {
        alSourcePause(_handle);        
    }

    void source::stop() const noexcept {
        alSourceStop(_handle);
    }

    void source::playAll(const source * pSources, std::size_t n) noexcept {
        unbox(pSources, n, [](auto pSources, auto n) {
            alSourcePlayv(static_cast<ALsizei> (n), pSources);
        });
    }

    void source::pauseAll(const source * pSources, std::size_t n) noexcept {
        unbox(pSources, n, [](auto pSources, auto n) { 
            alSourcePausev(static_cast<ALsizei> (n), pSources);
        });
    }

    void source::stopAll(const source * pSources, std::size_t n) noexcept {
        unbox(pSources, n, [](auto pSources, auto n) {
            alSourceStopv(static_cast<ALsizei> (n), pSources);
        });        
    }
}
