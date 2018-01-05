#include "graphics/sampler_info.hpp"

namespace graphics {
    template<>
    sampler_info defaults() noexcept {
        return {
            {graphics::mag_filter::LINEAR, graphics::min_filter::LINEAR_MIPMAP_LINEAR},
            {graphics::address_mode::REPEAT, graphics::address_mode::REPEAT, graphics::address_mode::REPEAT},
            {-1000.0F, 1000.0F}
        };
    }
}