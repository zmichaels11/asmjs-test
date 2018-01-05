#pragma once

#include <memory>
#include <string>

#include "audio/buffer.hpp"
#include "audio/format.hpp"
#include "audio/sound_channel.hpp"
#include "audio/source.hpp"

namespace audio {
    enum class sound_state {
        PLAYING,
        STOPPED,
        LOOPING
    };

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

        sound(const std::string& path, float loopOffset = 0.0F) noexcept; 

        void onFrame() noexcept;

        void setPosition(float x, float y, float z) noexcept;

        void setVelocity(float x, float y, float z) noexcept;

        void setDirection(float x, float y, float z) noexcept;

        void setGain(float value) noexcept;

        void setPitch(float value) noexcept;

        sound_state getState() const noexcept;

        void play() noexcept;

        void stop() noexcept;

        void loop() noexcept;
    };
}