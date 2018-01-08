#pragma once

#include <cstddef>

#include "graphics/henum/internal_format.hpp"

#include "graphics/hinfo/sampler_info.hpp"

namespace graphics {
    struct texture_info {
        struct dim_t {
            std::size_t width;
            std::size_t height;
            std::size_t depth;
        } extent;

        std::size_t layers;
        std::size_t levels;

        graphics::sampler_info samplerInfo;
        graphics::internal_format format;
    };
}