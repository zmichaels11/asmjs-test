#pragma once

#include "graphics/address_mode.hpp"
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

    inline void defaults(sampler_info& info) {
        info.filters.mag = graphics::mag_filter::LINEAR;
        info.filters.min = graphics::min_filter::LINEAR_MIPMAP_LINEAR;
        info.addressing.wrapS = graphics::address_mode::REPEAT;
        info.addressing.wrapT = graphics::address_mode::REPEAT;
        info.addressing.wrapR = graphics::address_mode::REPEAT;
        info.lod.min = -1000.0F;
        info.lod.max = 1000.0F;
    }
}