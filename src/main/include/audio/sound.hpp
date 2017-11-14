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
        sound(const std::string& path, float loopOffset = 0.0F);

        sound(sound&&) = default;

        sound& operator=(sound&&) = default;

        void onFrame();

        void setPosition(float x, float y, float z);

        void setVelocity(float x, float y, float z);

        void setDirection(float x, float y, float z);

        void setGain(float value);

        void setPitch(float value);

        sound_state getState() const;

        void play();

        void stop();

        void loop();
    };
}