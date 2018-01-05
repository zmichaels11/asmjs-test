#pragma once

#include <vector>

#include "audio/buffer.hpp"

namespace audio {
    class source {
        unsigned int _handle;

        source(const source&) = delete;

        source& operator=(const source&) = delete;

    public:
        source(source&&) = default;                

        source& operator=(source&&) = default;

        source() noexcept;

        ~source() noexcept;          

        void setGain(float value) const noexcept;

        void setPitch(float value) const noexcept;

        void setPosition(float x, float y, float z) const noexcept;

        void setVelocity(float x, float y, float z) const noexcept;

        void setDirection(float x, float y, float z) const noexcept;

        std::size_t getBuffersQueued() const noexcept;

        std::size_t getBuffersProcessed() const noexcept;

        void unqueueBuffers(buffer * pBuffers, std::size_t n) const noexcept;

        void queueBuffers(const buffer * pBuffers, std::size_t n = 1) const noexcept;

        void play() const noexcept;
    };
}