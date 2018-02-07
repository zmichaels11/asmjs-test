#pragma once

#include <cstddef>

#include "audio/format.hpp"

namespace audio {
    /**
     * A source of streamable sound data.
     */
    class sound_channel {
    public:
        virtual ~sound_channel() noexcept {}

        /**
         * Retrieves the number of audio channels.
         * 
         * @return the number of channels.
         */
        virtual int getChannels() const noexcept = 0;

        /**
         * Retrieves the frequency of the sample rate.
         * 
         * @return the frequency of samples.
         */
        virtual int getSampleRate() const noexcept = 0;

        /**
         * Retrieves the number of bits per sample.
         * 
         * @return the bits per sample.
         */
        virtual int getBitsPerSample() const noexcept = 0;
        
        /**
         * Retrieves the amount of bytes per second in the stream.
         * 
         * In the case of variable rate encodings, this refers to the rate post decode to PCM.
         * 
         * @return the byte rate of the audio stream.
         */
        virtual int getByteRate() const noexcept = 0;

        /**
         * Retrieves the equivalent format if applicable.
         * 
         * @return the audio format.
         */
        virtual format getFormat() const noexcept = 0;

        /**
         * Seeks to the first sample of the stream. 
         * 
         * This function has the same functionality of seek(0), but may introduce performance enhancements.
         */
        virtual void seekStart() noexcept = 0;

        /**
         * Seeks to a specific sample number.             
         */
        virtual void seek(unsigned int sample) noexcept = 0;

        /**
         * Retrieves the length of the audio stream in seconds.
         * 
         * @return the length of the audio stream.
         */
        virtual float getLength() const noexcept = 0;

        /**
         * Reads data from the stream.
         * 
         * @param dst the memory block to write data to.
         * @param n the amount of data to read in basic machine units. Also represents the amount of data actually read.
         * @return true if the stream still has data.
         */
        virtual bool read(void * dst, std::size_t& n) noexcept = 0;
    };
}
