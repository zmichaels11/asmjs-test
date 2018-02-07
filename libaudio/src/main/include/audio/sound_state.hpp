#pragma once

namespace audio {
    /**
     * Valid states of a sound object.
     */
    enum class sound_state {
        PLAYING,    /**< The sound object will only emit sound until the stream ends. */
        STOPPED,    /**< The sound object is not emitting any sound. */
        LOOPING     /**< The sound object will repeat from beginning when the sound completes. */
    };
}
