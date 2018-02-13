#pragma once

#include <cstddef>

#include <functional>
#include <memory>

#include "audio/buffer.hpp"
#include "audio/sound.hpp"
#include "audio/source.hpp"
#include "audio/vorbis_channel.hpp"
#include "audio/wave_file_channel.hpp"
#include "audio/wave_memory_channel.hpp"

namespace audio {
    /**
     * Initializes the audio library. 
     * This function may be executed multiple times. 
     * Subsequent calls may be ignored.
     */
    void init() noexcept;

    template<class ALobj_T>
    void box(
        ALobj_T * pObjects,
        std::size_t n,
        const std::function<void(unsigned int *, std::size_t)>& pfn) noexcept {

        auto tmp = std::make_unique<unsigned int[]> (n);

        pfn(tmp.get(), n);

        for (decltype(n) i = 0; i < n; i++) {
            pObjects[i] = ALobj_T(tmp[i]);
        }        
    }

    template<class ALobj_T>
    void unbox(
        const ALobj_T * pObjects, 
        std::size_t n,
        const std::function<void(unsigned int *, std::size_t)>& pfn) noexcept {

        auto tmp = std::make_unique<unsigned int[]> (n);

        for (decltype(n) i = 0; i < n; i++) {
            tmp[i] = static_cast<unsigned int> (pObjects[i]);
        }

        pfn(tmp.get(), n);
    }
}