#pragma once

#include "graphics/henum/address_mode.hpp"
#include "graphics/henum/mag_filter.hpp"
#include "graphics/henum/min_filter.hpp"

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

        float anisotropy;

        constexpr static sampler_info defaults() noexcept {
            return {
                filters: {
                    mag: graphics::mag_filter::LINEAR, 
                    min: graphics::min_filter::LINEAR_MIPMAP_LINEAR},
                addressing: {
                    wrapS: graphics::address_mode::REPEAT, 
                    wrapT: graphics::address_mode::REPEAT, 
                    wrapR: graphics::address_mode::REPEAT},                    
                lod: {
                    min: -1000.0F, 
                    max: 1000.0F},
                anisotropy: 1.0F
            };
        }
    };
}