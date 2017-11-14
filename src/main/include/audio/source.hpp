#pragma once

#include <AL/al.h>

#include <vector>

#include "audio/buffer.hpp"

namespace audio {
    class source {
        ALuint _handle;

        source(const source&) = delete;

        source& operator=(const source&) = delete;

    public:
        source();

        ~source();        

        source(source&&) = default;                

        source& operator=(source&&) = default;

        void setGain(float value) const;

        void setPitch(float value) const;

        void setPosition(float x, float y, float z) const;        

        void setVelocity(float x, float y, float z) const;

        void setDirection(float x, float y, float z) const;

        int getBuffersQueued() const;

        int unqueueBuffers(std::vector<buffer>& out) const;

        void queueBuffer(const buffer& buffer) const;

        void play() const;
    };
}