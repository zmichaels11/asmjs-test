#pragma once

namespace audio {
    enum class source_state : unsigned int {
        INITIAL = 0x1011,
        PLAYING = 0x1012,
        PAUSED = 0x1013,
        STOPPED = 0x1014
    };
}