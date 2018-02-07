#pragma once

#include "audio/buffer.hpp"
#include "audio/sound.hpp"
#include "audio/source.hpp"

namespace audio {
    /**
     * Initializes the audio library. 
     * This function may be executed multiple times. 
     * Subsequent calls may be ignored.
     */
    void init() noexcept;
}