#pragma once

#include <AL/al.h>

namespace audio {
    enum class format : ALenum {
        MONO16 = AL_FORMAT_MONO16,
        MONO8 = AL_FORMAT_MONO8,
        MONO_ALAW = 0x10016,
        MONO_ULAW = 0x10014,
        MONO_FLOAT32 = 0x10010,
        STEREO16 = AL_FORMAT_STEREO16,
        STEREO8 = AL_FORMAT_STEREO8,
        STEREO_ALAW = 0x10017,
        STEREO_ULAW = 0x10015,
        STEREO_FLOAT32 = 0x10011
    };    
}