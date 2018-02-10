#pragma once

#include <cstddef>

#include "audio/format.hpp"

namespace audio {
    struct buffer_info {
        unsigned int frequency;
        audio::format format;
        void * data;
        std::size_t size;
    };
}