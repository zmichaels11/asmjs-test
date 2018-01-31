#pragma once

#include <cstdint>

namespace engine {
    namespace layers {
        struct color {
            std::uint8_t r, g, b, a;

            static constexpr color rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept {
                return {r, g, b, static_cast<std::uint8_t> (255)};
            }

            static constexpr color rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept {
                return {r, g, b, a};
            }
        };
    }
}