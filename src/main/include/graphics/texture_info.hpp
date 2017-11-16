#pragma once

#include <cstddef>

#include "graphics/sampler_info.hpp"
#include "graphics/internal_format.hpp"

namespace graphics {
    struct texture_info {
        struct dim_t {
            std::size_t width;
            std::size_t height;
            std::size_t depth;
        } extent;

        std::size_t layers;
        std::size_t levels;

        sampler_info samplerInfo;
        internal_format format;
    };
}