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

        /**
         * Retrieves the amount of buffers queued up.
         * 
         * @return the amount of unprocessed buffers.
         */
        std::size_t getBuffersQueued() const noexcept;

        /**
         * Retrieves the amount of buffers processed.
         * 
         * @return the amount of processed buffers that are ready for recycle.
         */
        std::size_t getBuffersProcessed() const noexcept;

        /**
         * Removes n processed buffers from the queue.
         * 
         * @param pBuffers the array to write the buffers to.
         * @param n the amount of buffers to retrieve.
         */
        void unqueueBuffers(buffer * pBuffers, std::size_t n) const noexcept;

        /**
         * Adds n buffers to the end of the queue.
         * 
         * @param pBuffers the array of buffers to process.
         * @param n the amount of buffers to process. Default value is 1.
         */
        void queueBuffers(const buffer * pBuffers, std::size_t n = 1) const noexcept;

        /**
         * Begins playback of the source. 
         * 
         * This will play until the buffer queue is depleted.
         */
        void play() const noexcept;
    };
}
