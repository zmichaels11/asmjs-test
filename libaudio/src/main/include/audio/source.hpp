#pragma once

#include <cstddef>

#include "audio/source_state.hpp"

namespace audio {
    class buffer;

    /**
     * A source of sound.
     * 
     * A source object consumes buffer objects and emits audio.
     */
    class source {
        unsigned int _handle;

        source(const source&) = delete;

        source& operator=(const source&) = delete;

    public:
        source(source&&) = default;                

        source& operator=(source&&) = default;

        /**
         * Constructs a new source object.
         */
        source() noexcept;

        virtual ~source() noexcept;          

        /**
         * Sets the gain of the source.
         * 
         * @param value the gain.
         */
        void setGain(float value) const noexcept;

        /**
         * Sets the pitch of the source.
         * 
         * @param value the pitch.
         */
        void setPitch(float value) const noexcept;

        /**
         * Sets the position of the source in world space.
         * 
         * @param x the x-location.
         * @param y the y-location.
         * @param z the z-location.
         */
        void setPosition(float x, float y, float z) const noexcept;

        /**
         * Sets the velocity of the source.
         * 
         * @param x the x-location.
         * @param y the y-location.
         * @param z the z-location.
         */
        void setVelocity(float x, float y, float z) const noexcept;

        /**
         * Sets the direction of the source.
         * 
         * @param x the x-direction.
         * @param y the y-direction.
         * @param z the z-direction.
         */
        void setDirection(float x, float y, float z) const noexcept;

        std::size_t getProcessedBuffers() const noexcept;

        void unqueueBuffers(buffer * pBuffers, std::size_t nBuffers = 1) const noexcept;

        std::size_t unqueueBuffers() const noexcept;

        void queueBuffers(const buffer * pBuffers, std::size_t nBuffers = 1) const noexcept;

        void queueBuffer(audio::buffer&& buffer) const noexcept;        

        source_state getState() const noexcept;

        /**
         * Begins playback of the source. 
         * 
         * This will play until the buffer queue is depleted.
         */
        void play() const noexcept;

        void pause() const noexcept;

        void stop() const noexcept;

        static void playAll(const source * pSources, std::size_t n = 1) noexcept;

        static void pauseAll(const source * pSources, std::size_t n = 1) noexcept;

        static void stopAll(const source * pSources, std::size_t n = 1) noexcept;        

        inline operator unsigned int() const noexcept {
            return _handle;
        }
    };
}
