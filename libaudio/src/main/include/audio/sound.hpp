#pragma once

#include <memory>
#include <string>

#include "audio/buffer.hpp"
#include "audio/format.hpp"
#include "audio/sound_channel.hpp"
#include "audio/sound_state.hpp"
#include "audio/source.hpp"

namespace audio {    
    class sound {
        std::unique_ptr<sound_channel> _channel;
        format _format;
        int _sampleRate;
        buffer _buffers[3];
        source _source;
        sound_state _state;        
        std::size_t _bufferSize;
        unsigned int _loopSample;

        sound(const sound&) = delete;

        sound& operator=(const sound&) = delete;

    public:        
        sound(sound&&) = default;

        sound& operator=(sound&&) = default;

        /**
         * Constructs a new sound object.
         * 
         * @param path the location of the sound object.
         * @param loopOffset the position in seconds to return to on completion of a loop. Ignored if set to PLAY state.
         */
        sound(const std::string& path, float loopOffset = 0.0F) noexcept; 

        /**
         * Applies updates to the sound
         */
        void onFrame() noexcept;

        /**
         * Sets the relative position of the sound in world space.
         * 
         * @param x the x-location.
         * @param y the y-location.
         * @param z the z-location.
         */
        void setPosition(float x, float y, float z) noexcept;

        /**
         * Sets the velocity of the sound.
         *                    
         * @param x the x-velocity.
         * @param y the y-velocity.
         * @param z the z-velocity.
         */
        void setVelocity(float x, float y, float z) noexcept;

        /**
         * Sets the direction of the sound. 
         * 
         * Only applicable to directional sounds.
         * 
         * @param x the x-direction.
         * @param y the y-direction.
         * @param z the z-direction.
         */
        void setDirection(float x, float y, float z) noexcept;

        /**
         * Sets the gain of the audio.
         * 
         * The gain is the ratio of sound volume. 1.0 corresponds to full volume and 0.0 corresponds to silence.
         * Values greater than 1.0 will amplify the sound. 
         * There may be sound artifacts if the limits of the encoding are exceeded.
         * 
         * @param value the gain.
         */
        void setGain(float value) noexcept;

        /**
         * Sets the pitch of the audio.
         * 
         * This is the amount of distortion to the rate of playback to apply.
         * 
         * @param value the pitch.
         */
        void setPitch(float value) noexcept;

        /**
         * Retrieves the state of the sound.
         * 
         * @return the current state.
         */
        sound_state getState() const noexcept;

        /**
         * Plays the sound.
         * 
         * The sound object will begin emitting sound and enter the "PLAY" state.
         * The sound object will continue emitting sound until the attached stream reaches EOF.
         * Once the sound reaches EOF, the sound will enter the "STOP" state.
         */
        void play() noexcept;

        /**
         * Stops the sound.
         * 
         * The sound object will stop emitting sound and will enter the "STOP" state.
         */
        void stop() noexcept;

        /**
         * Loops the sound.
         * 
         * The sound object will begin emitting sound and enter the "LOOPING" state.
         * The sound object will seek to the loopOffset once the attached stream reaches EOF.
         */
        void loop() noexcept;
    };
}