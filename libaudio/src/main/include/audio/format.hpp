#pragma once

namespace audio {
    /**
     * Audio formats supported by the audio library.
     * 
     * All enum definitions evaluate to their corresponding OpenAL value as unsigned int.
     */
    enum class format : unsigned int {
        MONO16 = 0x1101,            /**< 1-channel 16bit unsigned integer PCM encoding. */
        MONO8 = 0x1100,             /**< 1-channel 8bit unsigned integer PCM encoding. */
        MONO_ALAW = 0x10016,        /**< 1-channel 8bit signed logarithmic ALAW encoding. */
        MONO_ULAW = 0x10014,        /**< 1-channel 8bit signed logarithmic ULAW encoding. */
        MONO_FLOAT32 = 0x10010,     /**< 1-channel 32bit signed float PCM encoding. */
        STEREO16 = 0x1103,          /**< 2-channel 16bit unsigned integer encoding. */
        STEREO8 = 0x1102,           /**< 2-channel 8bit unsigned integer encoding. */
        STEREO_ALAW = 0x10017,      /**< 2-channel 8bit signed logarithmic ALAW encoding. */
        STEREO_ULAW = 0x10015,      /**< 2-channel 8bit signed logarithmic ULAW encoding. */
        STEREO_FLOAT32 = 0x10011    /**< 2-channel 32bit signed float PCM encoding. */
    };    
}