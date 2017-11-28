#pragma once

#include <memory>
#include <string>

namespace graphics {
    class image;

    namespace image_io {
        std::unique_ptr<image> read(const std::string& path, unsigned int forcedChannels = 0);
    }
}