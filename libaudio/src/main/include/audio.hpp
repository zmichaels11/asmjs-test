#pragma once

namespace audio {
    /**
     * Initializes the audio library. 
     * This function may be executed multiple times. 
     * Subsequent calls may be ignored.
     */
    void init() noexcept;
}