#pragma once

#include <string>

#include "graphics/image.hpp"

namespace engine {
    class image_cache {
    public:
        static void gc() noexcept;

        static const graphics::image * getImage(const std::string& imgRef) noexcept;
    };
}