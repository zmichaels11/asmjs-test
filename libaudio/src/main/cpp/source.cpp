#include "audio/source.hpp"

#include <AL/al.h>

#include <cstdio>
#include <memory>
#include <vector>

#include "audio/buffer.hpp"

namespace audio {
    source::source() {
        alGenSources(1, &_handle);
    }

    source::~source() {
        alDeleteSources(1, &_handle);        
    }

    void source::setGain(float value) const {
        alSourcef(_handle, AL_GAIN, value);
    }

    void source::setPitch(float value) const {
        alSourcef(_handle, AL_PITCH, value);
    }

    void source::setPosition(float x, float y, float z) const {
        alSource3f(_handle, AL_POSITION, x, y, z);
    }

    void source::setVelocity(float x, float y, float z) const {
        alSource3f(_handle, AL_VELOCITY, x, y, z);
    }

    void source::setDirection(float x, float y, float z) const {
        alSource3f(_handle, AL_DIRECTION, x, y, z);
    }

    int source::getBuffersQueued() const {
        int out;

        alGetSourcei(_handle, AL_BUFFERS_QUEUED, &out);

        return out;
    }

    int source::unqueueBuffers(std::vector<buffer>& out) const {
        int nbuffers;

        alGetSourcei(_handle, AL_BUFFERS_PROCESSED, &nbuffers);

        if (nbuffers > 0) {            
            auto buffers = std::make_unique<ALuint[]> (nbuffers);
        
            alSourceUnqueueBuffers(_handle, nbuffers, buffers.get());        

            for (int i = 0; i < nbuffers; i++) {
                out.push_back(buffer(buffers[i]));
            }
        }        

        return nbuffers;
    }

    void source::queueBuffer(const buffer& buffer) const {
        auto bufferHandle = buffer._handle;

        alSourceQueueBuffers(_handle, 1, &bufferHandle);
    }

    void source::play() const {
        alSourcePlay(_handle);
    }
}