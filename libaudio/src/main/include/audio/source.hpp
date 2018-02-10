#pragma once

#include <cstddef>

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

        void queueBuffer(audio::buffer&& buffer) const noexcept;

        std::size_t gc() const noexcept;

        /**
         * Begins playback of the source. 
         * 
         * This will play until the buffer queue is depleted.
         */
        void play() const noexcept;
    };
}
