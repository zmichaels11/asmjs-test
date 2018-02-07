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
                {graphics::mag_filter::LINEAR, graphics::min_filter::LINEAR_MIPMAP_LINEAR},
                {
                    graphics::address_mode::REPEAT, 
                    graphics::address_mode::REPEAT, 
                    graphics::address_mode::REPEAT},                    
                {-1000.0F, 1000.0F},
                1.0F
            };
        }
    };
}