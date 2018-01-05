#pragma once

#include "graphics/address_mode.hpp"
#include "graphics/info.hpp"
#include "graphics/mag_filter.hpp"
#include "graphics/min_filter.hpp"

namespace graphics {
    struct sampler_info {
        struct filter_t {
            graphics::mag_filter mag;
            graphics::min_filter min;
        } filters;

        struct addressing_t {
            graphics::address_mode wrapS;
            graphics::address_mode wrapT;
            graphics::address_mode wrapR;
        } addressing;

        struct lod_t {
            float min;
            float max;
        } lod;
    };

    template<>
    sampler_info defaults() noexcept;

}