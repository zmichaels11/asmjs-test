#pragma once

#include <cstddef>

#include "engine/layers/sound_type.hpp"

namespace engine {
    namespace layers {
        struct sound_info {
            sound_type type;
            const char * data;
            std::size_t len;

            static constexpr sound_info ogg(const char * data, std::size_t len) noexcept {
                return {
                    sound_type::VORBIS,
                    data,
                    len
                };
            }

            static constexpr sound_info wave(const char * data, std::size_t len) noexcept {
                return {
                    sound_type::WAVE,
                    data,
                    len
                };
            }
        };
    }
}