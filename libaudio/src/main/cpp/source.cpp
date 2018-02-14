#include "pch.h"
#include "audio/source.hpp"

#include "audio.hpp"
#include "audio/buffer.hpp"

namespace audio {    
    source::source(const source_info& info) noexcept {
        alGenSources(1, &_handle);
        alSourcef(_handle, AL_GAIN, info.gain);
        alSourcef(_handle, AL_PITCH, info.pitch);
        alSource3f(_handle, AL_POSITION, info.position.x, info.position.y, info.position.z);
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

    std::size_t source::unqueueBuffers() const noexcept {
        ALint nBuffers = 0;

        alGetSourcei(_handle, AL_BUFFERS_PROCESSED, &nBuffers);

        switch (nBuffers) {
            case 0:
                return 0;
            case 1: {
                ALuint buffer = 0;

                alSourceUnqueueBuffers(_handle, 1, &buffer);
                alDeleteBuffers(1, &buffer);

                return 1;
            } break;
            default: {
                auto pBuffers = std::make_unique<ALuint[]> (nBuffers);

                alSourceUnqueueBuffers(_handle, nBuffers, pBuffers.get());
                alDeleteBuffers(nBuffers, pBuffers.get());
                
                return static_cast<std::size_t> (nBuffers);
            } break;
        }
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

    void source::queueBuffer(audio::buffer&& buffer) const noexcept {
        alSourceQueueBuffers(_handle, 1, &buffer._handle);        
        buffer._transient = true;
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
