#pragma once

namespace audio {
    enum class format : unsigned int {
        MONO16 = 0x1101,
        MONO8 = 0x1100,
        MONO_ALAW = 0x10016,
        MONO_ULAW = 0x10014,
        MONO_FLOAT32 = 0x10010,
        STEREO16 = 0x1103,
        STEREO8 = 0x1102,
        STEREO_ALAW = 0x10017,
        STEREO_ULAW = 0x10015,
        STEREO_FLOAT32 = 0x10011
    };    
}