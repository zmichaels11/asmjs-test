#pragma once

#include "graphics/address_mode.hpp"
#include "graphics/mag_filter.hpp"
#include "graphics/min_filter.hpp"

namespace graphics {
    struct sampler_info {
        struct filter_t {
            mag_filter mag;
            min_filter min;
        } filters;

        struct addressing_t {
            address_mode wrapS;
            address_mode wrapT;
            address_mode wrapR;
        } addressing;

        struct lod_t {
            float min;
            float max;
        } lod;
    };
}